/*
 * file_organizer.c
 * ----------------
 * Organizes files in a directory by their extension into categorized folders.
 * Unrecognized extensions are moved to an "Others/" folder.
 *
 * Compile : gcc -o file_organizer file_organizer.c
 * Usage   : ./file_organizer <target_directory>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#ifdef _WIN32
  #include <io.h>          /* _access, F_OK */
  #include <direct.h>      /* _mkdir        */
  #define access  _access
  #define F_OK    0
  #define mkdir(p, m) _mkdir(p)
#else
  #include <unistd.h>      /* access, F_OK  */
#endif

/* ------------------------------------------------------------------ */
/*  Category table                                                      */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *folder;         /* destination sub-folder name */
    const char *extensions[25]; /* NULL-terminated list of extensions  */
} Category;

static const Category CATEGORIES[] = {
    { "Images",     { ".jpg", ".jpeg", ".png", ".gif", ".bmp",
                      ".svg", ".webp", ".ico", ".tiff", ".raw", NULL } },
    { "Videos",     { ".mp4", ".mkv", ".avi", ".mov", ".wmv",
                      ".flv", ".webm", ".mpeg", ".3gp", NULL } },
    { "Audio",      { ".mp3", ".wav", ".flac", ".aac", ".ogg",
                      ".wma", ".m4a", ".opus", NULL } },
    { "Documents",  { ".pdf", ".doc", ".docx", ".xls", ".xlsx",
                      ".ppt", ".pptx", ".odt", ".txt", ".rtf",
                      ".csv", ".epub", NULL } },
    { "Archives",   { ".zip", ".tar", ".gz", ".bz2", ".rar",
                      ".7z", ".xz", ".tgz", NULL } },
    { "Code",       { ".c", ".h", ".cpp", ".py", ".js", ".ts",
                      ".html", ".css", ".java", ".go", ".rs",
                      ".sh", ".bat", ".php", ".rb", ".json",
                      ".xml", ".yaml", ".yml", ".sql", NULL } },
    { "Fonts",      { ".ttf", ".otf", ".woff", ".woff2", ".eot", NULL } },
    { "Executables",{ ".exe", ".msi", ".deb", ".rpm", ".apk",
                      ".dmg", ".out", NULL } },
    { NULL,         { NULL } }   /* sentinel */
};

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

/* Convert a string to lowercase in-place */
static void to_lower(char *s) {
    for (; *s; ++s)
        if (*s >= 'A' && *s <= 'Z') *s += 32;
}

/* Extract the extension (including the dot) from a filename.
   Returns pointer into filename, or NULL if no extension. */
static const char *get_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return NULL;
    return dot;
}

/* Create a directory (does nothing if it already exists). */
static int make_dir(const char *path) {
    int ret = mkdir(path, 0755);
    if (ret != 0 && errno != EEXIST) {
        perror(path);
        return -1;
    }
    return 0;
}

/* Build a path: "<base>/<name>" into dst (caller supplies buffer). */
static void build_path(char *dst, size_t size,
                        const char *base, const char *name) {
    snprintf(dst, size, "%s/%s", base, name);
}

/* ------------------------------------------------------------------ */
/*  Core logic                                                          */
/* ------------------------------------------------------------------ */

/*
 * Look up the destination folder for a given (lowercase) extension.
 * Returns "Others" if not found.
 */
static const char *find_folder(const char *ext_lower) {
    for (int i = 0; CATEGORIES[i].folder != NULL; ++i) {
        for (int j = 0; CATEGORIES[i].extensions[j] != NULL; ++j) {
            if (strcmp(ext_lower, CATEGORIES[i].extensions[j]) == 0)
                return CATEGORIES[i].folder;
        }
    }
    return "Others";
}

/*
 * Move src_path → dest_dir/filename.
 * If a file with the same name already exists in dest_dir,
 * appends _1, _2, … before the extension until a free slot is found.
 */
static int move_file(const char *src_path, const char *dest_dir,
                     const char *filename) {
    char dest_path[1024];
    build_path(dest_path, sizeof(dest_path), dest_dir, filename);

    /* Handle name collision */
    if (access(dest_path, F_OK) == 0) {
        char base[512], ext[64] = "";
        const char *dot = get_extension(filename);
        if (dot) {
            strncpy(ext, dot, sizeof(ext) - 1);
            size_t base_len = (size_t)(dot - filename);
            strncpy(base, filename, base_len);
            base[base_len] = '\0';
        } else {
            strncpy(base, filename, sizeof(base) - 1);
        }

        int counter = 1;
        do {
            snprintf(dest_path, sizeof(dest_path),
                     "%s/%s_%d%s", dest_dir, base, counter, ext);
            ++counter;
        } while (access(dest_path, F_OK) == 0);
    }

    if (rename(src_path, dest_path) != 0) {
        fprintf(stderr, "  [ERROR] Could not move '%s' → '%s': %s\n",
                src_path, dest_path, strerror(errno));
        return -1;
    }

    printf("  [OK] %-40s → %s\n", filename, dest_path);
    return 0;
}

/*
 * Organise all files inside `dir_path`.
 */
static void organize_directory(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror(dir_path);
        return;
    }

    int moved = 0, skipped = 0, errors = 0;

    printf("\n=== File Organizer ===\n");
    printf("Target: %s\n\n", dir_path);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;

        /* Skip hidden files, ".", ".." */
        if (name[0] == '.') continue;

        /* Build full source path */
        char src_path[1024];
        build_path(src_path, sizeof(src_path), dir_path, name);

        /* Skip sub-directories */
        struct stat st;
        if (stat(src_path, &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) {
            printf("  [SKIP] '%s' is a directory — skipping.\n", name);
            ++skipped;
            continue;
        }

        /* Determine destination folder */
        const char *ext = get_extension(name);
        const char *folder_name = "Others";

        if (ext) {
            char ext_lower[64];
            strncpy(ext_lower, ext, sizeof(ext_lower) - 1);
            ext_lower[sizeof(ext_lower) - 1] = '\0';
            to_lower(ext_lower);
            folder_name = find_folder(ext_lower);
        }

        /* Create destination folder if needed */
        char dest_dir[1024];
        build_path(dest_dir, sizeof(dest_dir), dir_path, folder_name);
        if (make_dir(dest_dir) != 0) { ++errors; continue; }

        /* Move the file */
        if (move_file(src_path, dest_dir, name) == 0)
            ++moved;
        else
            ++errors;
    }

    closedir(dir);

    printf("\n--- Summary ---\n");
    printf("  Moved  : %d file(s)\n", moved);
    printf("  Skipped: %d item(s)\n", skipped);
    printf("  Errors : %d\n", errors);
    printf("Done.\n\n");
}

/* ------------------------------------------------------------------ */
/*  Entry point                                                         */
/* ------------------------------------------------------------------ */

int main(int argc, char *argv[]) {
    const char *target = (argc >= 2) ? argv[1] : ".";
    organize_directory(target);
    return EXIT_SUCCESS;
}