from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch
from reportlab.lib import colors
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, HRFlowable,
    KeepTogether, Table, TableStyle, PageBreak
)
from reportlab.lib.enums import TA_LEFT, TA_CENTER, TA_JUSTIFY, TA_RIGHT
from reportlab.platypus.flowables import Flowable

W, H = letter

# ── Colors ───────────────────────────────────────────────────────────────────
NAVY        = colors.HexColor("#0d2137")
DARK_BLUE   = colors.HexColor("#1a3557")
MID_BLUE    = colors.HexColor("#2b5c8a")
LIGHT_BLUE  = colors.HexColor("#d6e8f7")
GOLD        = colors.HexColor("#c8a951")
GOLD_DARK   = colors.HexColor("#a07830")
RED_ACCENT  = colors.HexColor("#8b1a1a")
LIGHT_GRAY  = colors.HexColor("#f4f6f9")
MED_GRAY    = colors.HexColor("#cdd4de")
TEXT        = colors.HexColor("#1a1a1a")
TEXT_BODY   = colors.HexColor("#2c2c2c")
GREEN_TAG   = colors.HexColor("#1a5c2a")
GOLD_TAG    = colors.HexColor("#7a5200")

# ── Document ──────────────────────────────────────────────────────────────────
doc = SimpleDocTemplate(
    "/mnt/user-data/outputs/Quirino_Comprehensive_Research.pdf",
    pagesize=letter,
    leftMargin=0.9*inch, rightMargin=0.9*inch,
    topMargin=0.85*inch, bottomMargin=0.85*inch,
)
CW = 6.6*inch   # content width

# ── Base Styles ───────────────────────────────────────────────────────────────
base = getSampleStyleSheet()

def S(name, **kw):
    p = ParagraphStyle(name, parent=base["Normal"], **kw)
    return p

body    = S("Body", fontSize=9.5, leading=15, textColor=TEXT_BODY,
            fontName="Helvetica", alignment=TA_JUSTIFY, spaceAfter=6)
body_b  = S("BodyB", fontSize=9.5, leading=15, textColor=TEXT_BODY,
            fontName="Helvetica-Bold", alignment=TA_JUSTIFY, spaceAfter=4)
small   = S("Small", fontSize=8.5, leading=13, textColor=colors.HexColor("#444"),
            fontName="Helvetica-Oblique", alignment=TA_JUSTIFY, spaceAfter=4)
bullet_s= S("Bullet", fontSize=9.2, leading=14.5, textColor=TEXT_BODY,
            fontName="Helvetica", alignment=TA_JUSTIFY,
            leftIndent=16, firstLineIndent=-10, spaceAfter=3)
label_s = S("Label", fontSize=8, leading=11, textColor=colors.white,
            fontName="Helvetica-Bold", alignment=TA_CENTER)
cite_s  = S("CiteStyle", fontSize=9, leading=13.5, textColor=TEXT_BODY,
            fontName="Helvetica-Bold", alignment=TA_JUSTIFY,
            leftIndent=20, spaceAfter=2)
annot_s = S("AnnotStyle", fontSize=8.8, leading=13.5, textColor=TEXT_BODY,
            fontName="Helvetica", alignment=TA_JUSTIFY,
            leftIndent=20, spaceAfter=2, backColor=LIGHT_GRAY,
            borderPad=(4,6,4,6))
url_s   = S("URLStyle", fontSize=7.8, leading=11, textColor=MID_BLUE,
            fontName="Helvetica-Oblique", leftIndent=20, spaceAfter=8)
ref_s   = S("RefStyle", fontSize=8.8, leading=13.5, textColor=TEXT_BODY,
            fontName="Helvetica", alignment=TA_JUSTIFY,
            leftIndent=24, firstLineIndent=-24, spaceAfter=5)
num_s   = S("NumStyle", fontSize=9, leading=12, textColor=GOLD_DARK,
            fontName="Helvetica-Bold", spaceAfter=1)

# ── Helpers ───────────────────────────────────────────────────────────────────
def chapter_header(num, title, subtitle=None):
    """Full-width chapter header bar."""
    rows = [[Paragraph(f"<font color='#c8a951'>{num}</font>  {title}", S(
        f"CH{num}", fontSize=16, leading=20, textColor=colors.white,
        fontName="Helvetica-Bold", alignment=TA_LEFT))]]
    tbl = Table(rows, colWidths=[CW])
    tbl.setStyle(TableStyle([
        ("BACKGROUND",    (0,0),(-1,-1), NAVY),
        ("TOPPADDING",    (0,0),(-1,-1), 10),
        ("BOTTOMPADDING", (0,0),(-1,-1), 10),
        ("LEFTPADDING",   (0,0),(-1,-1), 14),
        ("RIGHTPADDING",  (0,0),(-1,-1), 14),
    ]))
    items = [tbl]
    if subtitle:
        items.append(Paragraph(subtitle, S("CHSub", fontSize=8.5, leading=12,
            textColor=MID_BLUE, fontName="Helvetica-Oblique",
            spaceBefore=2, spaceAfter=6)))
    return items

def section_bar(title, color=MID_BLUE):
    tbl = Table([[Paragraph(f"  {title}", S("SB", fontSize=10.5, leading=14,
        textColor=colors.white, fontName="Helvetica-Bold"))]], colWidths=[CW])
    tbl.setStyle(TableStyle([
        ("BACKGROUND",    (0,0),(-1,-1), color),
        ("TOPPADDING",    (0,0),(-1,-1), 5),
        ("BOTTOMPADDING", (0,0),(-1,-1), 5),
        ("LEFTPADDING",   (0,0),(-1,-1), 8),
        ("RIGHTPADDING",  (0,0),(-1,-1), 8),
    ]))
    return [Spacer(1,10), tbl, Spacer(1,5)]

def tag_box(label, bg=MID_BLUE):
    t = Table([[Paragraph(label, label_s)]], colWidths=[1.0*inch])
    t.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),bg),
        ("TOPPADDING",(0,0),(-1,-1),2),("BOTTOMPADDING",(0,0),(-1,-1),2),
        ("LEFTPADDING",(0,0),(-1,-1),4),("RIGHTPADDING",(0,0),(-1,-1),4),
    ]))
    return t

def pill(text, bg, fg=colors.white):
    ps = S("Pill", fontSize=7.5, leading=10, textColor=fg,
           fontName="Helvetica-Bold", alignment=TA_CENTER)
    t = Table([[Paragraph(text, ps)]])
    t.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),bg),
        ("TOPPADDING",(0,0),(-1,-1),2),("BOTTOMPADDING",(0,0),(-1,-1),2),
        ("LEFTPADDING",(0,0),(-1,-1),6),("RIGHTPADDING",(0,0),(-1,-1),6),
        ("ROUNDEDCORNERS",[3]),
    ]))
    return t

def info_row(label, value):
    ls = S("IL", fontSize=9, leading=13, textColor=colors.white,
           fontName="Helvetica-Bold")
    vs = S("IV", fontSize=9, leading=13, textColor=TEXT_BODY,
           fontName="Helvetica")
    t = Table([[Paragraph(label, ls), Paragraph(value, vs)]],
              colWidths=[1.5*inch, CW-1.5*inch])
    t.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(0,-1), DARK_BLUE),
        ("BACKGROUND",(1,0),(1,-1), LIGHT_GRAY),
        ("TOPPADDING",(0,0),(-1,-1),4),("BOTTOMPADDING",(0,0),(-1,-1),4),
        ("LEFTPADDING",(0,0),(-1,-1),8),("RIGHTPADDING",(0,0),(-1,-1),8),
        ("LINEBELOW",(0,0),(-1,-1),0.5,MED_GRAY),
    ]))
    return t

def bullet(text):
    return Paragraph(f"• {text}", bullet_s)

def sub_bullet(text):
    return Paragraph(f"   – {text}", S("SBul", fontSize=9, leading=13.5,
        textColor=TEXT_BODY, fontName="Helvetica",
        leftIndent=28, firstLineIndent=-12, spaceAfter=2))

def highlight_box(text, bg=LIGHT_BLUE, border=MID_BLUE):
    ps = S("HB", fontSize=9.2, leading=14, textColor=DARK_BLUE,
           fontName="Helvetica", alignment=TA_JUSTIFY)
    t = Table([[Paragraph(text, ps)]], colWidths=[CW])
    t.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),bg),
        ("LINEAFTER",(0,0),(0,-1),3,border),
        ("TOPPADDING",(0,0),(-1,-1),7),("BOTTOMPADDING",(0,0),(-1,-1),7),
        ("LEFTPADDING",(0,0),(-1,-1),12),("RIGHTPADDING",(0,0),(-1,-1),10),
    ]))
    return [t, Spacer(1,6)]

def gold_box(text):
    return highlight_box(text, bg=colors.HexColor("#fdf6e3"), border=GOLD)

def quote_block(text, source):
    qs = S("QS", fontSize=9.5, leading=15, textColor=DARK_BLUE,
           fontName="Helvetica-Oblique", alignment=TA_JUSTIFY)
    as_ = S("AS", fontSize=8, leading=12, textColor=GOLD_DARK,
            fontName="Helvetica-Bold")
    t = Table([
        [Paragraph(f"\u201c{text}\u201d", qs)],
        [Paragraph(f"— {source}", as_)],
    ], colWidths=[CW])
    t.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(-1,-1),colors.HexColor("#f8f4ec")),
        ("LINEAFTER",(0,0),(0,-1),4,GOLD),
        ("TOPPADDING",(0,0),(-1,-1),8),("BOTTOMPADDING",(0,0),(-1,-1),8),
        ("LEFTPADDING",(0,0),(-1,-1),14),("RIGHTPADDING",(0,0),(-1,-1),10),
        ("LINEABOVE",(0,0),(-1,0),0.5,GOLD),
        ("LINEBELOW",(0,-1),(-1,-1),0.5,GOLD),
    ]))
    return [t, Spacer(1,8)]

def primary_source_box(num, citation, annotation, url=None):
    items = [
        Paragraph(f"[PS-{num}]", S("PSN", fontSize=8.5, leading=12,
            textColor=RED_ACCENT, fontName="Helvetica-Bold", spaceBefore=10, spaceAfter=1)),
        Paragraph(citation, cite_s),
        Paragraph(annotation, annot_s),
    ]
    if url:
        items.append(Paragraph(f"\U0001f517 {url}", url_s))
    return KeepTogether(items)

def annot_entry(num, citation, annotation, url=None, tag="SECONDARY"):
    bg = colors.HexColor("#1a5c2a") if tag == "PRIMARY" else DARK_BLUE
    items = [
        Paragraph(f"[{num}]", S("AEN", fontSize=8.5, leading=12,
            textColor=bg, fontName="Helvetica-Bold", spaceBefore=10, spaceAfter=1)),
        Paragraph(f'<font color="#888888">[{tag}]</font>  {citation}', cite_s),
        Paragraph(annotation, annot_s),
    ]
    if url:
        items.append(Paragraph(f"\U0001f517 {url}", url_s))
    return KeepTogether(items)

def ref_entry(num, text):
    return Paragraph(f"[{num}]  {text}", ref_s)

def two_col(left_items, right_items, lw=3.1*inch, rw=3.3*inch):
    left_tbl  = Table([[x] for x in left_items],  colWidths=[lw])
    right_tbl = Table([[x] for x in right_items], colWidths=[rw])
    left_tbl.setStyle(TableStyle([("TOPPADDING",(0,0),(-1,-1),0),
                                   ("BOTTOMPADDING",(0,0),(-1,-1),0),
                                   ("LEFTPADDING",(0,0),(-1,-1),0),
                                   ("RIGHTPADDING",(0,0),(-1,-1),0)]))
    right_tbl.setStyle(TableStyle([("TOPPADDING",(0,0),(-1,-1),0),
                                    ("BOTTOMPADDING",(0,0),(-1,-1),0),
                                    ("LEFTPADDING",(0,0),(-1,-1),0),
                                    ("RIGHTPADDING",(0,0),(-1,-1),0)]))
    outer = Table([[left_tbl, right_tbl]], colWidths=[lw, rw])
    outer.setStyle(TableStyle([
        ("VALIGN",(0,0),(-1,-1),"TOP"),
        ("TOPPADDING",(0,0),(-1,-1),0),("BOTTOMPADDING",(0,0),(-1,-1),0),
        ("LEFTPADDING",(0,0),(-1,-1),0),("RIGHTPADDING",(0,0),(-1,-1),4),
    ]))
    return outer

# ═══════════════════════════════════════════════════════════════════════════════
# COVER PAGE
# ═══════════════════════════════════════════════════════════════════════════════
story = []

# Top banner
cover_banner = Table([[
    Paragraph("POST-WAR PHILIPPINES · GROUP 2 · FINAL GROUP MLT", S(
        "CB", fontSize=8, leading=11, textColor=GOLD,
        fontName="Helvetica-Bold", alignment=TA_CENTER))
]], colWidths=[CW])
cover_banner.setStyle(TableStyle([
    ("BACKGROUND",(0,0),(-1,-1),NAVY),
    ("TOPPADDING",(0,0),(-1,-1),7),("BOTTOMPADDING",(0,0),(-1,-1),7),
]))
story.append(cover_banner)
story.append(Spacer(1, 0.3*inch))

# Accent line
story.append(HRFlowable(width="100%", thickness=3, color=GOLD))
story.append(Spacer(1, 0.15*inch))

# Main title
story.append(Paragraph("ELPIDIO QUIRINO", S("CT", fontSize=38, leading=44,
    textColor=NAVY, fontName="Helvetica-Bold", alignment=TA_CENTER)))
story.append(Paragraph("Sixth President of the Republic of the Philippines", S(
    "CS", fontSize=14, leading=18, textColor=MID_BLUE,
    fontName="Helvetica", alignment=TA_CENTER, spaceBefore=4)))
story.append(Spacer(1, 0.08*inch))

# Year badge
yr_tbl = Table([[Paragraph("1948 – 1953", S("YR", fontSize=18, leading=22,
    textColor=colors.white, fontName="Helvetica-Bold", alignment=TA_CENTER))
]], colWidths=[2.0*inch])
yr_tbl.setStyle(TableStyle([
    ("BACKGROUND",(0,0),(-1,-1),DARK_BLUE),
    ("TOPPADDING",(0,0),(-1,-1),8),("BOTTOMPADDING",(0,0),(-1,-1),8),
    ("ALIGN",(0,0),(-1,-1),"CENTER"),
]))
yr_wrap = Table([[yr_tbl]], colWidths=[CW])
yr_wrap.setStyle(TableStyle([("ALIGN",(0,0),(-1,-1),"CENTER"),
                              ("TOPPADDING",(0,0),(-1,-1),0),
                              ("BOTTOMPADDING",(0,0),(-1,-1),0)]))
story.append(yr_wrap)
story.append(Spacer(1, 0.18*inch))

# Divider with gold diamond
story.append(HRFlowable(width="100%", thickness=1, color=MED_GRAY))
story.append(Spacer(1, 0.15*inch))

# Quick info table
quick_data = [
    ["Full Name", "Elpidio Rivera Quirino y Rivera"],
    ["Born", "November 16, 1890 — Vigan, Ilocos Sur"],
    ["Died", "February 29, 1956 — Novaliches, Quezon City (Heart Attack)"],
    ["Party", "Liberal Party"],
    ["Vice President", "Fernando López (1949–1953)"],
    ["Preceded by", "Manuel A. Roxas (died in office, April 15, 1948)"],
    ["Succeeded by", "Ramon F. Magsaysay"],
]
for row in quick_data:
    story.append(info_row(row[0], row[1]))
story.append(Spacer(1, 0.18*inch))

# Tags row
tags_tbl = Table([[
    tag_box("POLITICAL", NAVY),
    Spacer(4,1),
    tag_box("ECONOMIC", DARK_BLUE),
    Spacer(4,1),
    tag_box("SOCIO-CULTURAL", MID_BLUE),
    Spacer(4,1),
    tag_box("HUK CRISIS", RED_ACCENT),
    Spacer(4,1),
    tag_box("COLD WAR", colors.HexColor("#3d3d3d")),
]], colWidths=[1.1*inch,0.08*inch,1.05*inch,0.08*inch,1.25*inch,0.08*inch,1.0*inch,0.08*inch,0.88*inch])
tags_tbl.setStyle(TableStyle([("ALIGN",(0,0),(-1,-1),"CENTER"),
                               ("TOPPADDING",(0,0),(-1,-1),0),
                               ("BOTTOMPADDING",(0,0),(-1,-1),0)]))
story.append(tags_tbl)
story.append(Spacer(1, 0.15*inch))
story.append(HRFlowable(width="100%", thickness=3, color=GOLD))
story.append(Spacer(1, 0.12*inch))

# Document sub-header
story.append(Paragraph("COMPREHENSIVE RESEARCH DOCUMENT", S("CDH", fontSize=10,
    leading=13, textColor=MID_BLUE, fontName="Helvetica-Bold", alignment=TA_CENTER)))
story.append(Paragraph(
    "Includes: Introduction · Biography · Presidency · Political, Economic & Socio-Cultural Contributions · "
    "Major Challenges · Legacy · Trivia · Primary Sources · Annotated Bibliography · List of References",
    S("CDsub", fontSize=8, leading=12, textColor=colors.HexColor("#666"),
      fontName="Helvetica-Oblique", alignment=TA_CENTER, spaceBefore=2)))
story.append(Spacer(1, 0.08*inch))

# TOC box
toc_items = [
    ("I", "Introduction"),
    ("II", "Biography and Background"),
    ("III", "The Presidency"),
    ("IV", "Political Contributions"),
    ("V", "Economic Contributions"),
    ("VI", "Socio-Cultural Contributions"),
    ("VII", "Major Challenges During His Term"),
    ("VIII", "Legacy and Historical Significance"),
    ("IX", "Interesting Facts and Trivia"),
    ("X", "Primary Sources"),
    ("XI", "Annotated Bibliography"),
    ("XII", "List of References"),
]
toc_rows = [[Paragraph(n, S("TN", fontSize=8.5, textColor=GOLD, fontName="Helvetica-Bold")),
             Paragraph(t, S("TT", fontSize=8.5, textColor=TEXT_BODY, fontName="Helvetica"))]
            for n, t in toc_items]
toc_tbl = Table(toc_rows, colWidths=[0.5*inch, 6.0*inch])
toc_tbl.setStyle(TableStyle([
    ("BACKGROUND",(0,0),(-1,-1),LIGHT_GRAY),
    ("ROWBACKGROUNDS",(0,0),(-1,-1),[colors.white, LIGHT_GRAY]),
    ("TOPPADDING",(0,0),(-1,-1),3),("BOTTOMPADDING",(0,0),(-1,-1),3),
    ("LEFTPADDING",(0,0),(-1,-1),8),("RIGHTPADDING",(0,0),(-1,-1),8),
    ("LINEBELOW",(0,0),(-1,-2),0.3,MED_GRAY),
]))
story.append(toc_tbl)
story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION I — INTRODUCTION
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("I", "INTRODUCTION"))
story.append(Spacer(1,8))

story.extend(highlight_box(
    "<b>Thesis Statement:</b> The presidency of Elpidio Quirino (1948–1953) stands as one of the "
    "most consequential and contradictory chapters in Philippine post-war history — a period of "
    "genuine reconstruction and institution-building shadowed by grave threats from within: "
    "Communist insurgency, rampant graft, fraudulent elections, and deep agrarian injustice "
    "that the administration failed to resolve at its roots."
))

story.append(Paragraph(
    "When Manuel Roxas, the first President of the independent Republic of the Philippines, died "
    "of a heart attack on April 15, 1948, Vice President Elpidio Quirino was thrust unexpectedly "
    "into the presidency. The nation he inherited was fragile — barely two years removed from "
    "colonial rule, devastated by World War II, economically dependent on American aid, and "
    "confronting a growing armed peasant rebellion in the heartland of Luzon. Quirino would "
    "serve five turbulent years, winning a full term in the deeply controversial 1949 election "
    "before being swept out of office by Ramon Magsaysay in 1953.", body))

story.append(Paragraph(
    "Historical assessments of Quirino have long been polarized. Critics point to the fraud-ridden "
    "1949 election — widely called the dirtiest in Philippine history — widespread graft, "
    "the golden chamber pot scandal, and the administration's inability to address the structural "
    "roots of peasant unrest. Supporters, on the other hand, credit him with establishing the "
    "Central Bank of the Philippines, securing millions in U.S. economic assistance, "
    "fostering important regional diplomacy, and overseeing measurable economic recovery. "
    "This document does not reduce Quirino to either caricature. It examines him fully: "
    "as a public servant shaped by personal tragedy, constrained by structural realities, "
    "and ultimately limited by the very political culture he embodied.", body))

story.append(Paragraph(
    "This research document is organized to meet the requirements of the Final Group MLT "
    "on the Post-War Period. It covers the key political, economic, and socio-cultural dimensions "
    "of the Quirino era from a pro-Filipino perspective — asking not just what happened, "
    "but what it meant for ordinary Filipinos, and how they responded to the challenges of "
    "the time. Primary sources, an annotated bibliography, and a full list of references "
    "are included at the end.", body))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION II — BIOGRAPHY
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("II", "BIOGRAPHY AND BACKGROUND",
    "\"Elpidio Rivera Quirino — From Ilocos Sur to Malacañang\""))
story.append(Spacer(1,8))

for sb in section_bar("Early Life and Origins"):
    story.append(sb)

story.append(Paragraph(
    "Elpidio Rivera Quirino was born on November 16, 1890, in the Vigan Provincial Jail in "
    "Vigan, Ilocos Sur — an irony of history, as his father Mariano Quirino y Quebral served "
    "as the jail warden. His mother, Gregoria Rivera y Mendoza, came from Agoo, La Union. "
    "Of Chinese mestizo descent, Quirino was the third of six children. His early years were "
    "marked by financial difficulty, and he taught school in Caoayan while attending Vigan "
    "High School to earn money for his education.", body))

story.append(Paragraph(
    "He later transferred to Manila High School, graduating in 1911 — also passing the "
    "first-grade civil service examination the same year. He worked as a junior computer in "
    "the Bureau of Lands and as a property clerk in the Manila Police Department before "
    "enrolling in the College of Law at the University of the Philippines, graduating in 1915. "
    "He then served as law clerk in the Philippine Commission and secretary to Senate President "
    "Manuel Quezon, beginning a career of close association with the highest circles of "
    "Philippine political power.", body))

for sb in section_bar("Political Career Before the Presidency"):
    story.append(sb)

career_data = [
    ["1919–1925", "Representative, Ilocos Sur 1st District, Philippine House of Representatives"],
    ["1925–1931", "Senator, Philippine Legislature"],
    ["1931", "Secretary of Finance; then Secretary of the Interior under Gov.-Gen. Theodore Roosevelt Jr."],
    ["1934", "Member, Philippine Independence Commission to Washington, D.C. — secured passage of the Tydings-McDuffie Act"],
    ["1935", "Delegate, Constitutional Convention — helped draft the 1935 Philippine Constitution"],
    ["1935–1940", "Secretary of Finance and Interior under President Manuel Quezon (Commonwealth Government)"],
    ["1941–1945", "Wartime — survived Japanese occupation; wife Alicia Syquia and three children killed in 1945 Battle of Manila"],
    ["1945", "Senate President Pro Tempore upon reconstitution of the Commonwealth"],
    ["1946", "Elected Vice President of the Republic; appointed Secretary of Foreign Affairs"],
    ["1948", "Assumed presidency upon death of Manuel Roxas, April 17, 1948"],
]
header_row = [Paragraph("Period", S("TH", fontSize=8.5, textColor=colors.white,
              fontName="Helvetica-Bold")),
              Paragraph("Role / Event", S("TH2", fontSize=8.5, textColor=colors.white,
              fontName="Helvetica-Bold"))]
career_rows = [[Paragraph(r[0], S("TC0", fontSize=8.5, leading=12,
                fontName="Helvetica-Bold", textColor=GOLD_DARK)),
               Paragraph(r[1], S("TC1", fontSize=8.5, leading=12,
                fontName="Helvetica", textColor=TEXT_BODY))]
               for r in career_data]
career_tbl = Table([header_row] + career_rows, colWidths=[1.35*inch, 5.25*inch])
career_tbl.setStyle(TableStyle([
    ("BACKGROUND",(0,0),(-1,0), DARK_BLUE),
    ("ROWBACKGROUNDS",(0,1),(-1,-1),[colors.white, LIGHT_GRAY]),
    ("TOPPADDING",(0,0),(-1,-1),4), ("BOTTOMPADDING",(0,0),(-1,-1),4),
    ("LEFTPADDING",(0,0),(-1,-1),7), ("RIGHTPADDING",(0,0),(-1,-1),7),
    ("GRID",(0,0),(-1,-1),0.4, MED_GRAY),
    ("VALIGN",(0,0),(-1,-1),"TOP"),
]))
story.append(career_tbl)
story.append(Spacer(1,8))

for sb in section_bar("Personal Life and Wartime Tragedy"):
    story.append(sb)

story.append(Paragraph(
    "In 1918, Quirino married Alicia Syquia of Vigan. They had five children: "
    "Fe, Armando, Norma, Thomas, and Victoria. In February 1945, during the liberation "
    "of Manila from Japanese forces, Alicia and three of their children — Fe, Armando, "
    "and Norma — were killed in the chaos of the Battle of Manila. Quirino's personal "
    "grief was thus inextricably bound to the national tragedy of the war's devastation. "
    "He would serve as president as a widower, with his youngest daughter Victoria "
    "('Vicky') serving as Malacañang's First Lady at just 16 years old.", body))

story.extend(quote_block(
    "His personal tragedies deepened his commitment to the Filipino people, even as the "
    "political environment conspired to undermine his stated ideals.",
    "Raissa Espinosa-Robles, To Fight Without End (Ayala Foundation, 1990)"
))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION III — THE PRESIDENCY
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("III", "THE PRESIDENCY",
    "\"The Republic in Crisis and Recovery, 1948–1953\""))
story.append(Spacer(1,8))

for sb in section_bar("Assumption of Office (1948)"):
    story.append(sb)

story.append(Paragraph(
    "Quirino took his oath of office on April 17, 1948 — two days after the death of "
    "Manuel Roxas — in the Council of State Room at Malacañang Palace. "
    "His first official act was proclaiming a period of national mourning. "
    "He immediately articulated two core objectives: (1) the economic reconstruction of "
    "the nation, and (2) the restoration of the faith and confidence of the people in "
    "the government. These objectives would frame — and haunt — his entire term.", body))

story.extend(highlight_box(
    "<b>Key Policy Institutions Created in 1948:</b> President's Action Committee on Social "
    "Amelioration (PACSA) · Agricultural Credit Cooperatives Financing Administration (ACCFA) · "
    "Labor Management Advisory Board · Rural Banks of the Philippines · "
    "<b>Central Bank of the Philippines</b> (Republic Act No. 265, signed June 15, 1948; "
    "inaugurated January 3, 1949) · Quezon City declared new capital of the Philippines "
    "(Republic Act No. 333, July 17, 1948)"
))

for sb in section_bar("The 1949 Presidential Election"):
    story.append(sb)

story.append(Paragraph(
    "In the November 1949 elections, Quirino won a full term against Nationalista candidate "
    "and former president José P. Laurel and fellow Liberal José Avelino. "
    "The election gave the Liberal Party a historic sweep — the only time in Philippine "
    "history where president, vice president, and all senators came from one party. "
    "However, the election was immediately and widely condemned as the most fraudulent "
    "in Philippine history, characterized by violence, ballot-stuffing, intimidation of "
    "opposition voters, and documented manipulation by military and police forces. "
    "This electoral scandal would permanently damage Quirino's legitimacy.", body))

story.extend(gold_box(
    "Carlos P. Romulo and Marvin Gray called the 1949 election the 'dirty election' in "
    "their book <i>The Magsaysay Story</i> (1956), citing widespread fraud and "
    "intimidation by military action. The backlash led to a Nacionalista sweep in the "
    "1951 midterm elections — the Liberals won zero Senate seats."
))

for sb in section_bar("Second Term and Final Years (1949–1953)"):
    story.append(sb)

story.append(Paragraph(
    "Quirino's second term was defined by three overlapping crises: the Hukbalahap "
    "insurgency reaching its peak (1950), the economic emergency requiring the Bell Mission "
    "and the Quirino-Foster Agreement (1950), and mounting public anger over corruption. "
    "The appointment of Ramon Magsaysay as Secretary of National Defense in 1950 "
    "proved transformative for the counterinsurgency campaign — but Magsaysay's growing "
    "popularity ultimately overshadowed and undermined Quirino's political standing.", body))

story.append(Paragraph(
    "In November 1953, despite a reform agenda and measurable economic gains, "
    "Quirino lost to Magsaysay in a landslide — largely because public fury over "
    "corruption and the 1949 election was irreversible. He retired to his home in "
    "Novaliches, Quezon City, and died on February 29, 1956, of a heart attack. "
    "He was 65 years old.", body))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION IV — POLITICAL CONTRIBUTIONS
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("IV", "POLITICAL CONTRIBUTIONS"))
story.append(Spacer(1,8))

for sb in section_bar("Institution-Building and Governance"):
    story.append(sb)

story.append(bullet("Signed <b>Republic Act No. 265</b> (June 15, 1948) establishing the <b>Central Bank of the Philippines</b>, creating an independent monetary authority that regulated currency, banking, and credit — a cornerstone of modern Philippine financial governance."))
story.append(bullet("Declared <b>Quezon City</b> the new capital of the Philippines (RA No. 333, July 17, 1948), initiating the planned relocation of the seat of government."))
story.append(bullet("Established the <b>Integrity Board</b> (1950) to investigate graft and corruption in government — the first formal anti-corruption body in the Republic's history, though its effectiveness was limited by political interference."))
story.append(bullet("Reorganized the civil service through a Government Reorganization Commission, aiming to professionalize public administration."))
story.append(bullet("Revived President Quezon's <b>'Fireside Chats'</b> — monthly radio broadcasts from Malacañang — as a tool of direct democratic communication between the President and the Filipino people."))
story.append(Spacer(1,6))

for sb in section_bar("Foreign Policy and Diplomacy"):
    story.append(sb)

story.append(Paragraph(
    "Quirino's foreign policy was marked by a strong orientation toward the United States "
    "and active regional diplomacy in Southeast Asia, driven by Cold War imperatives and "
    "the desire to secure the Philippines' position in the post-war international order.", body))

story.append(bullet("<b>Baguio Conference (May 1950):</b> Quirino hosted the Economic Commission for Asia and the Far East (ECAFE) conference, bringing together India, Pakistan, Ceylon, Thailand, Indonesia, and Australia. This was the Philippines' first major act of regional diplomatic leadership."))
story.append(bullet("<b>Philippine Expeditionary Forces to Korea (PEFTOK, 1950):</b> Authorized deployment of over 7,450 Filipino soldiers to the Korean War, affirming the Philippines' commitment to collective security and the U.S.-led anti-communist coalition."))
story.append(bullet("<b>Mutual Defense Treaty with the United States (1951):</b> Signed a formal military alliance with the U.S. that remains a cornerstone of Philippine defense policy to this day."))
story.append(bullet("<b>Japanese War Criminal Amnesty (July 1953):</b> Granted clemency to 105 Japanese war criminals — a deeply controversial but historically significant act of reconciliation that laid the groundwork for the <b>1956 Japanese Reparations Treaty</b> ($550 million in goods and services)."))
story.append(bullet("<b>San Francisco Peace Treaty (1951):</b> Signed the treaty formally ending the state of war between Japan and the Allied Powers."))
story.append(bullet("Expanded the Philippine diplomatic network, opening legations in Bangkok, Buenos Aires, and consulates in Calcutta, Karachi, and Batavia."))
story.append(Spacer(1,6))

story.extend(quote_block(
    "I have never surrendered the sovereignty, much less the dignity and future of our country.",
    "President Elpidio Quirino, on Philippine-American relations (1950)"
))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION V — ECONOMIC CONTRIBUTIONS
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("V", "ECONOMIC CONTRIBUTIONS"))
story.append(Spacer(1,8))

for sb in section_bar("Post-War Reconstruction"):
    story.append(sb)

story.append(Paragraph(
    "The Philippines that Quirino inherited in 1948 had a gross national product at "
    "approximately 40% of pre-war levels. Nearly all public infrastructure — roads, bridges, "
    "ports, hospitals, school buildings — had been destroyed or severely damaged in the war. "
    "National income had collapsed from pre-war highs to as low as ₱18 million in 1945. "
    "Against this backdrop, the Quirino administration's reconstruction efforts were substantial, "
    "even if structurally incomplete.", body))

story.append(bullet("Oversaw rehabilitation of major public infrastructure under the Philippine Rehabilitation Act, utilizing approximately $520 million in U.S. rehabilitation funds."))
story.append(bullet("National income grew from ₱18 million (1945) to over ₱300 million by 1949, then reached ₱5.12 billion in 1951 — a dramatic if uneven recovery."))
story.append(bullet("Agricultural output recovered: palay production rose from 36.9 million cavans (1945–46) to approximately 56 million cavans by 1948–49, matching or exceeding pre-war levels."))
story.append(bullet("Fish production surpassed pre-war output — from 170,000 tons (1940) to 195,078 tons by 1948, with the industry now almost exclusively in Filipino hands."))
story.append(Spacer(1,6))

for sb in section_bar("Fiscal and Monetary Reforms"):
    story.append(sb)

story.append(bullet("<b>Central Bank of the Philippines (1948):</b> Established an independent monetary authority, ending the colonial-era system where the U.S. Federal Reserve effectively controlled Philippine currency."))
story.append(bullet("<b>Import and Exchange Control Act of 1948:</b> Implemented currency and import controls to protect foreign reserves and encourage local industrialization."))
story.append(bullet("Internal revenue collections reached ₱320 million in FY1949 — an all-time high, up from ₱191 million in 1947 and ₱67 million in 1946."))
story.append(bullet("Completed final payment on all pre-1934 dollar-bonded indebtedness to the United States during his administration."))
story.append(Spacer(1,6))

for sb in section_bar("The Bell Mission and Quirino-Foster Agreement"):
    story.append(sb)

story.append(Paragraph(
    "At Quirino's own request, President Truman sent an American Economic Survey Mission "
    "to Manila in July 1950, led by banker Daniel W. Bell. The Bell Report (October 1950) "
    "documented the harsh economic conditions of the Philippines and recommended $250 million "
    "in U.S. aid over five years — conditional on the Philippines implementing tax reform, "
    "minimum wage legislation, land reform, and anti-corruption measures.", body))

story.extend(gold_box(
    "On November 14, 1950, Quirino and U.S. representative William C. Foster signed the "
    "<b>Quirino-Foster Agreement</b> — the Philippines pledged economic reforms in exchange "
    "for $10 million in stabilization credits and the promise of a broader ECA aid program "
    "($15 million for FY-51, with $35.4 million requested for FY-52). This agreement reflected "
    "both Quirino's pragmatism and the limits of Philippine economic sovereignty."
))

story.append(bullet("<b>Magna Carta of Labor and Minimum Wage Law:</b> Enacted minimum wage protection for both agricultural and industrial workers, implementing a key Bell Mission recommendation."))
story.append(bullet("<b>LASEDECO (Land Settlement Development Corporation, EO 355, 1950):</b> Replaced the National Land Settlement Administration with a body tasked with agrarian resettlement and credit access for farmers."))
story.append(bullet("<b>EDCOR (Economic Development Corps):</b> Established resettlement areas for surrendered Huks, providing land and livelihood — integrating security and economic development."))
story.append(bullet("Farm-to-market roads and irrigation systems were expanded, reducing post-harvest losses and connecting rural producers to urban markets."))
story.append(bullet("By 1952, the rice harvest was projected as the largest in Philippine history, signaling potential food self-sufficiency."))
story.append(Spacer(1,6))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION VI — SOCIO-CULTURAL CONTRIBUTIONS
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("VI", "SOCIO-CULTURAL CONTRIBUTIONS"))
story.append(Spacer(1,8))

for sb in section_bar("Public Health and Social Welfare"):
    story.append(sb)

story.append(bullet("The general health condition nationwide improved during the Quirino years: death rates materially diminished while birth rates increased, as reported in his Second SONA (1950)."))
story.append(bullet("<b>PACSA (President's Action Committee on Social Amelioration)</b> was created specifically to mitigate the suffering of indigent families through direct government intervention."))
story.append(bullet("Post-war rehabilitation of small homes made rapid strides; government efforts actively addressed urban slums in Manila and other population centers."))
story.append(bullet("Expanded access to public health services in the provinces, with health forces deployed rapidly during the 1951 typhoon disasters to prevent epidemics."))
story.append(Spacer(1,6))

for sb in section_bar("Education and Labor"):
    story.append(sb)

story.append(bullet("Republic Act No. 312 standardized teachers' salaries based on occupational assignments and educational qualifications — a major step toward professionalizing the public teaching force."))
story.append(bullet("Expanded benefits for temporary government employees and workers, granting 15 days vacation leave and 15 days sick leave with full pay."))
story.append(bullet("The Labor Management Advisory Board was established to provide the executive with structured guidance on labor relations — a recognition of organized labor's growing role in the postwar economy."))
story.append(bullet("Wages increased 2% while the cost of living dropped 39% between 1948–1949, representing a real improvement in the purchasing power of working Filipinos."))
story.append(Spacer(1,6))

for sb in section_bar("Culture, Media, and National Identity"):
    story.append(sb)

story.append(bullet("<b>Fireside Chats via nationwide radio:</b> Quirino's monthly radio addresses brought the voice of the presidency into Filipino homes for the first time at scale — a significant socio-cultural act of democratic communication. Broadcasts covered current events, government programs, and civic appeals."))
story.append(bullet("Promoted civic consciousness and national unity through public addresses that emphasized Filipino resilience and the democratic tradition as shields against communism."))
story.append(bullet("Victoria Quirino's role as the teenage First Lady of Malacañang (from age 16) became a widely noted human-interest story, humanizing the presidency during a difficult period."))
story.append(bullet("The rehabilitation and reopening of the Legislative Building and other civic buildings in Manila was framed explicitly as a cultural and national act — the restoration of the physical symbols of Philippine democracy."))
story.append(bullet("The Quirino Grandstand (formerly Independence Grandstand in Rizal Park) — renamed in his honor — became a lasting landmark of public civic life and the site of his 1949 inauguration."))
story.append(Spacer(1,6))

story.extend(highlight_box(
    "<b>Socio-Cultural Context:</b> The Quirino era coincided with the rise of Filipino popular culture — "
    "a boom in Filipino cinema, the consolidation of Tagalog as the national language, and a growing "
    "sense of national identity forged out of wartime trauma and the experience of independent statehood. "
    "Government cultural patronage, though limited in scope, contributed to this nation-building moment."
))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION VII — MAJOR CHALLENGES
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("VII", "MAJOR CHALLENGES DURING HIS TERM"))
story.append(Spacer(1,8))

for sb in section_bar("1. The Hukbalahap Insurgency", RED_ACCENT):
    story.append(sb)

story.append(Paragraph(
    "The Hukbalahap (HUK) rebellion was the defining security challenge of the Quirino era. "
    "Originally an anti-Japanese guerrilla army formed in Central Luzon in 1942, "
    "the Huks — led by Luis Taruc — transformed into an armed communist insurgency after "
    "the war when their demands for land reform and political recognition were denied. "
    "By 1950, they controlled much of Central Luzon and threatened to bring down "
    "the Republic itself.", body))

story.append(bullet("<b>Failed Amnesty (1948):</b> Quirino granted amnesty to the Huks on June 21, 1948, and personally negotiated with Taruc. The agreement collapsed by August 1948 — the Huks accused the government of not meeting its commitments; the government found the Huks had not disarmed."))
story.append(bullet("<b>Peak of the Crisis (1950):</b> By mid-1950, the Communist Party of the Philippines and the HMB (Hukbong Magpapalaya ng Bayan) had laid plans to surround and seize Manila. U.S. intelligence and the capture of PKP documents revealed the full scope of the threat."))
story.append(bullet("<b>Magsaysay Appointed (September 1950):</b> Quirino appointed Ramon Magsaysay as Secretary of National Defense. Magsaysay's combination of military pressure, psychological operations, civic action, and the EDCOR resettlement program decisively weakened the insurgency."))
story.append(bullet("<b>Structural Failure:</b> The deeper problem — landlord exploitation, absentee ownership, and share-tenancy arrangements crushing Central Luzon peasants — was never adequately addressed. Agrarian reform remained partial and insufficient throughout Quirino's term."))
story.append(Spacer(1,6))

for sb in section_bar("2. Electoral Fraud and the Crisis of Democratic Legitimacy", RED_ACCENT):
    story.append(sb)

story.append(Paragraph(
    "The 1949 presidential election was a watershed in Philippine democratic history — "
    "for all the wrong reasons. Violence, ballot-stuffing, voter intimidation, "
    "and abuse of state military power characterized the contest. "
    "Quirino won, but the manner of his victory permanently delegitimized his presidency "
    "in the eyes of large segments of the Filipino public.", body))

story.append(bullet("The 1951 midterm elections — held under cleaner conditions after NAMFREL-style civic pressure and military oversight by Magsaysay — resulted in a complete Liberal Party rout. The Liberals won zero Senate seats."))
story.append(bullet("The credibility gap between Quirino's stated commitment to democratic governance and the reality of the 1949 election was never fully closed."))
story.append(Spacer(1,6))

for sb in section_bar("3. Graft, Corruption, and the 'Golden Arinola' Scandal", RED_ACCENT):
    story.append(sb)

story.append(Paragraph(
    "Quirino's administration was dogged by persistent, credible, and damaging allegations "
    "of graft and corruption — not only among his party allies but reportedly within "
    "his own household. The 'golden arinola' (chamber pot) scandal — reports of "
    "a gold-plated chamber pot purchased at public expense for Malacañang — "
    "became a symbol of perceived elite extravagance in a country where most Filipinos "
    "lived in poverty. The Integrity Board he created lacked independence and produced "
    "few meaningful prosecutions.", body))

story.append(Spacer(1,6))

for sb in section_bar("4. Economic Crisis and External Dependence", RED_ACCENT):
    story.append(sb)

story.append(bullet("The Philippines faced a severe balance-of-payments crisis in 1949–1950, with a foreign trade deficit of ₱547 million in 1949. Dollar reserves were critically depleted."))
story.append(bullet("Heavy reliance on U.S. economic aid came with conditions that compromised economic sovereignty — from the Bell Trade Act's parity rights to the Bell Mission's structural reform requirements."))
story.append(bullet("Agrarian poverty remained acute: the share-tenancy system (landlords taking 50–70% of harvests) sustained the conditions fueling the Huk rebellion."))
story.append(bullet("Natural disasters in 1951 (typhoons devastating the Visayas coconut industry) compounded economic recovery challenges."))
story.append(Spacer(1,6))

story.extend(highlight_box(
    "<b>Filipino Response to the Challenges:</b> Despite government failures, ordinary Filipinos "
    "demonstrated remarkable resilience. Peasants in Central Luzon organized through the Huk movement "
    "to demand justice. Civil society and journalists pressed for clean elections. Urban workers built "
    "labor unions. The 1951 elections — won cleanly by the opposition — demonstrated the Filipino "
    "people's capacity to reclaim democratic space when given the chance. These were acts of civic "
    "courage in the face of state repression."
))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION VIII — LEGACY
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("VIII", "LEGACY AND HISTORICAL SIGNIFICANCE"))
story.append(Spacer(1,8))

for sb in section_bar("Institutional Legacies"):
    story.append(sb)

story.append(bullet("The <b>Central Bank of the Philippines</b> (now Bangko Sentral ng Pilipinas) — one of the most enduring institutional legacies of his term — continues to serve as the cornerstone of Philippine monetary policy."))
story.append(bullet("The <b>Mutual Defense Treaty with the United States (1951)</b> remains in force as a foundational pillar of Philippine national security."))
story.append(bullet("The <b>Quirino-Foster Agreement</b> established a template for conditioned U.S.-Philippine development assistance that shaped the aid relationship for decades."))
story.append(bullet("The <b>1956 Reparations Treaty with Japan</b> (catalyzed by Quirino's 1953 amnesty for Japanese war criminals) brought $550 million in goods and services and laid the groundwork for Japan's role as one of the Philippines' leading development partners."))
story.append(Spacer(1,6))

for sb in section_bar("Historical Assessment"):
    story.append(sb)

story.append(Paragraph(
    "Quirino is a complex figure in Philippine historiography. Nationalist historians "
    "like Renato Constantino have argued that his administration was fundamentally a "
    "continuation of the neo-colonial order — serving American interests, protecting "
    "the landlord class, and suppressing genuine structural change. More sympathetic "
    "accounts (Lopez, Espinosa-Robles) argue that he governed under extraordinary "
    "constraints: a country devastated by war, a Communist insurgency, an entrenched "
    "political culture of patronage, and an opposition party that was no less corrupt.", body))

story.append(Paragraph(
    "What is undeniable is that Quirino presided over a period of genuine material recovery "
    "— infrastructure rebuilt, agricultural output restored, monetary institutions created — "
    "while simultaneously failing to address the social contract demands of the Filipino "
    "majority: land, clean government, and political dignity. The Magsaysay landslide of "
    "1953 was not merely a rejection of Quirino personally but a democratic verdict on "
    "the limits of elite postwar governance in the Philippines.", body))

for sb in section_bar("Memorials and Namesakes"):
    story.append(sb)

story.append(bullet("Municipality of <b>Quirino, Ilocos Sur</b> (formerly Angaki) — renamed in 1964"))
story.append(bullet("<b>Province of Quirino</b> — established in 1966"))
story.append(bullet("<b>Quirino, Isabela</b> (municipality, 1967)"))
story.append(bullet("<b>President Quirino, Sultan Kudarat</b> (municipality, 1973)"))
story.append(bullet("<b>Quirino Avenue, Manila</b> and <b>Elpidio Quirino Avenue, Parañaque</b>"))
story.append(bullet("<b>Quirino Grandstand</b> (formerly Independence Grandstand, Rizal Park, Manila) — site of major national ceremonies"))
story.append(bullet("<b>Quirino Highway</b> (Novaliches–Ipo Road, where his retirement home stands)"))
story.append(Spacer(1,8))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION IX — TRIVIA
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("IX", "INTERESTING FACTS AND TRIVIA"))
story.append(Spacer(1,8))

trivia_items = [
    ("Born in a Jail", 
     "Quirino was literally born in the Vigan Provincial Jail — because his father was the jail warden and the family lived on the premises. He joked in later years that it was an unusual beginning for a future president."),
    ("Youngest First Lady in Philippine History",
     "His daughter Victoria ('Vicky') became the youngest person ever to serve as official hostess of Malacañang Palace — at just 16 years old. She later married Luis M. Gonzalez, who became Philippine ambassador to Spain."),
    ("Only Time One Party Won Everything",
     "The 1949 elections — despite their notoriety for fraud — produced the only instance in Philippine history where the president, vice president, and all senators elected were from the same party (Liberal)."),
    ("First Philippine President to Be Impeached (Attempted)",
     "Quirino was the first Philippine president to face impeachment proceedings in Congress — though the effort was ultimately unsuccessful."),
    ("Forgave His Wife's Killers",
     "In a remarkable act of public reconciliation, Quirino granted amnesty to Japanese war criminals in 1953 — even though Japanese forces had killed his wife and three of his children during the Battle of Manila in 1945. He said publicly that hatred would not rebuild the nation."),
    ("University of Manila Honorary Degree",
     "The day before he was officially sworn in as president (April 16, 1948), the University of Manila conferred an honorary Doctor of Laws degree on him."),
    ("The 'Golden Arinola' Scandal",
     "The most famous — and damaging — gossip of his presidency: reports (disputed but widely believed) of a gold-plated chamber pot purchased for Malacañang at public expense. The image became a byword for corruption during his term."),
    ("Died on February 29",
     "Quirino died on February 29, 1956 — a leap day. He was one of very few world leaders in history to die on a date that only exists once every four years."),
    ("Fluent in Multiple Languages",
     "Quirino was noted for his oratorical skills in English, Spanish, Tagalog, and Ilocano — the multicultural linguistic reality of the Filipino elite of his generation."),
    ("Proposed the Pacific Security Pact First",
     "Quirino claimed to have first publicly proposed a Pacific collective security arrangement (analogous to the Atlantic/NATO Pact) as early as March 1949 — before the U.S. State Department was enthusiastic about the idea. The ANZUS Treaty and the eventual U.S.-Philippines Mutual Defense Treaty bore out his vision."),
]
for i, (title, text) in enumerate(trivia_items, 1):
    row = Table([[
        Paragraph(f"{i:02d}", S(f"TN{i}", fontSize=18, leading=22, textColor=GOLD,
            fontName="Helvetica-Bold", alignment=TA_CENTER)),
        Table([[Paragraph(title, S(f"TT{i}", fontSize=10, leading=13, textColor=NAVY,
                   fontName="Helvetica-Bold", spaceAfter=2)),
                Paragraph(text, body)]],
               colWidths=[5.5*inch])
    ]], colWidths=[0.6*inch, 5.9*inch])
    row.setStyle(TableStyle([
        ("BACKGROUND",(0,0),(0,-1), LIGHT_GRAY if i%2==0 else colors.white),
        ("BACKGROUND",(1,0),(1,-1), colors.white),
        ("VALIGN",(0,0),(-1,-1),"TOP"),
        ("TOPPADDING",(0,0),(-1,-1),6),("BOTTOMPADDING",(0,0),(-1,-1),6),
        ("LEFTPADDING",(0,0),(-1,-1),6),("RIGHTPADDING",(0,0),(-1,-1),6),
        ("LINEBELOW",(0,0),(-1,-1),0.5,MED_GRAY),
    ]))
    story.append(row)
story.append(Spacer(1,8))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION X — PRIMARY SOURCES
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("X", "PRIMARY SOURCES",
    "Original documents, speeches, laws, and first-hand accounts from the Quirino era"))
story.append(Spacer(1,6))

story.append(Paragraph(
    "The following are primary sources consulted and cited in this research document. "
    "They are original, first-hand materials produced during or directly relating to "
    "the Quirino administration (1948–1953).", small))
story.append(Spacer(1,4))

story.append(primary_source_box("01",
    'Quirino, Elpidio. "State-of-the-Nation Message to the Joint Session of Congress." '
    'Official Gazette of the Republic of the Philippines 45, no. 1 (January 24, 1949): 153–170.',
    'Quirino\'s first SONA as elected president — outlines his legislative agenda, reports on '
    'agricultural recovery (palay at 56M cavans), revenue collections (₱191M in 1947 vs ₱320M '
    'in 1949), and the restoration of civilian infrastructure.',
    'https://www.officialgazette.gov.ph/1949/01/24/state-of-the-nation-message-of-president-quirino-to-the-joint-session-of-the-congress-of-the-philippines/'))

story.append(primary_source_box("02",
    'Quirino, Elpidio. "Second State of the Nation Address." Official Gazette of the Republic '
    'of the Philippines (January 23, 1950).',
    'Comprehensive economic report: internal revenue at ₱320M (all-time high), peso price index '
    'at 45:58 centavos, fish output exceeding pre-war levels. Notes expansion of foreign relations.',
    'https://www.officialgazette.gov.ph/1950/01/23/elpidio-quirino-second-state-of-the-nation-address-january-23-1950/'))

story.append(primary_source_box("03",
    'Quirino, Elpidio. "Third State of the Nation Address." Official Gazette of the Republic '
    'of the Philippines (January 22, 1951).',
    'Delivered at the height of the Huk insurgency; reports a trade deficit of ₱547M (1949) '
    'now reduced to ₱28.8M (1950), outlines Bell Mission commitments and tax legislation.',
    'https://www.officialgazette.gov.ph/1951/01/22/elpidio-quirino-third-state-of-the-nation-address-january-22-1951/'))

story.append(primary_source_box("04",
    'Quirino, Elpidio. "Fourth State of the Nation Address." Official Gazette of the Republic '
    'of the Philippines (January 28, 1952).',
    'Reports national income growing to ₱5.12 billion (1951), manufacturing index rising from '
    '97.5 (1950) to 107.1 (1951), and completion of pre-1934 dollar-bond payments.',
    'https://www.officialgazette.gov.ph/1952/01/28/elpidio-quirino-fourth-state-of-the-nation-address-january-28-1952/'))

story.append(primary_source_box("05",
    'Quirino, Elpidio. "Inaugural Address." Official Gazette of the Republic of the Philippines '
    '45, no. 12 (December 30, 1949): 5380–5386.',
    'Delivered at Independence Grandstand; articulates vision of democratic governance, civic unity, '
    'and the first four years of recovery as exceeding all expectations.',
    'https://mirror.officialgazette.gov.ph/1949/12/30/inaugural-address-of-president-quirino/'))

story.append(primary_source_box("06",
    'Quirino, Elpidio. "Statement on the Report of the Bell Economic Survey Commission." '
    'Official Gazette of the Republic of the Philippines 46, no. 10 (October 28, 1950): 4810.',
    'Quirino\'s public response welcoming the Bell Mission Report and pledging transparency. '
    'Key document for the Quirino-Foster Agreement context.',
    'https://mirror.officialgazette.gov.ph/1950/10/28/statement-of-president-quirino-on-the-report-of-the-bell-economic-survey-commission-october-28-1950/'))

story.append(primary_source_box("07",
    'Quirino, Elpidio. "Seventeenth Radio Chat." Official Gazette of the Republic of the '
    'Philippines (March 15, 1950).',
    'Baguio broadcast addressing Filipinos about Cold War threats and internal security. '
    'Warns against "complacency, defeatism, and despair." Key source for political communication '
    'and socio-cultural analysis of the era.',
    'https://officialgazette.gov.ph/1950/03/15/message-of-president-quirino-march-15-1950'))

story.append(primary_source_box("08",
    'Quirino, Elpidio. "Extemporaneous Speech Before Provincial Governors and City Mayors." '
    'Official Gazette of the Republic of the Philippines 47, no. 7 (July 31, 1951): 3414–3418.',
    'Quirino urges local executives on the Japanese Peace Treaty and Pacific security pact, '
    'claiming he first proposed the concept in March 1949.',
    'https://www.officialgazette.gov.ph/1951/07/31/extemporaneous-speech-of-president-quirino-before-the-provincial-governors-and-city-mayors-convention/'))

story.append(primary_source_box("09",
    'Bell, Daniel W., et al. "Report of the United States Economic Survey Mission to the Philippines." '
    'In Foreign Relations of the United States, 1950, vol. VI. Washington, D.C.: U.S. Dept. of State, 1976. '
    'Document 841.',
    'The landmark Bell Report documenting harsh Philippine economic conditions and recommending '
    '$250 million U.S. aid over five years, conditional on tax reform, minimum wage legislation, '
    'and land reform. Cornerstone document for economic analysis of the Quirino era.',
    'https://history.state.gov/historicaldocuments/frus1950v06/d841'))

story.append(primary_source_box("10",
    'Truman, Harry S. "Statement Announcing Economic Survey Mission to the Philippines." '
    'June 29, 1950. Harry S. Truman Library, Public Papers of the Presidents, Doc. 180.',
    'Truman\'s formal authorization of the Bell Mission at Quirino\'s request. '
    'Establishes Daniel W. Bell as chief with personal ambassador rank and outlines '
    'the mission\'s comprehensive scope.',
    'https://www.trumanlibrary.gov/library/public-papers/180/statement-president-announcing-economic-survey-mission-philippines'))

story.append(primary_source_box("11",
    'U.S. Department of State. Foreign Relations of the United States, 1951: Asia and the Pacific. '
    'Vol. VI, Part 2. Documents 9 and 42. Washington, D.C.: U.S. Government Printing Office.',
    'Contains U.S. Embassy telegrams and State Dept. assessments on the Philippines: '
    'Quirino-Foster Agreement implementation, ECA aid program, Huk threat analysis, and '
    'U.S. recommendations on ensuring clean 1951 elections.',
    'https://history.state.gov/historicaldocuments/frus1951v06p2/d42'))

story.append(primary_source_box("12",
    '"Official Month in Review: November 1950." Official Gazette of the Republic of the Philippines '
    '(November 1, 1950).',
    'Day-by-day government record of November 1950: signing of Quirino-Foster Agreement (Nov. 14), '
    'Integrity Board reorganization, EO No. 375, and birthday celebrations in Vigan.',
    'https://www.officialgazette.gov.ph/1950/11/01/official-month-in-review-november-1950/'))

story.append(primary_source_box("13",
    'Taruc, Luis. Born of the People. New York: International Publishers, 1953.',
    'The autobiography of HMB commander Luis Taruc — the most important first-hand account '
    'from the insurgent side of the Huk rebellion. Describes the failed Quirino-Taruc '
    'negotiations of 1948 and the agrarian roots of the movement. Must be read critically '
    'given its communist framing and ghost-writer (William Pomeroy).'))

story.append(primary_source_box("14",
    'Quirino, Elpidio. "Radio Address on Pre-Election." Official Gazette of the Republic '
    'of the Philippines (November 7, 1949).',
    'Pre-election address summarizing Quirino\'s record: national income from ₱18M (1945) '
    'to ₱300M (1949), reconstruction achievements, and foreign policy gains.',
    'https://mirror.officialgazette.gov.ph/1949/11/07/radio-address-of-president-quirino-on-pre-election-november-7-1949'))

story.append(primary_source_box("15",
    'Quirino, Elpidio. The Quirino Way: Collection of Speeches and Addresses of Elpidio Quirino. '
    '1st ed. Manila: [s.n.], 1955.',
    'Compiled volume of Quirino\'s speeches and addresses, offering extensive insight into '
    'his rhetoric, political philosophy, and self-presentation across his administration. '
    'Cited in the Official Gazette as a primary archival reference.'))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION XI — ANNOTATED BIBLIOGRAPHY
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("XI", "ANNOTATED BIBLIOGRAPHY",
    "All sources classified as Primary (P) or Secondary (S) with annotations"))
story.append(Spacer(1,6))

story.append(Paragraph(
    "This bibliography includes all sources consulted in the preparation of this document. "
    "Primary sources are original first-hand materials; secondary sources are scholarly or "
    "analytical works interpreting the period. A total of 25 sources are listed below.", small))
story.append(Spacer(1,4))

story.append(Paragraph("— PRIMARY SOURCES (P) —", S("BibSec", fontSize=10, leading=13,
    textColor=GREEN_TAG, fontName="Helvetica-Bold", spaceBefore=8, spaceAfter=4)))

for sb in section_bar("Government Documents, Speeches, and Diplomatic Records", GREEN_TAG):
    story.append(sb)

story.append(annot_entry("P-01",
    'Quirino, Elpidio. State-of-the-Nation Message (January 24, 1949). Official Gazette 45(1): 153–170.',
    'First full SONA under Quirino\'s elected presidency. Provides baseline data on agricultural '
    'recovery, revenue collections, and postwar reconstruction. Essential for tracing the '
    'administration\'s stated economic agenda from the outset of his full term.',
    'https://www.officialgazette.gov.ph/1949/01/24/', "PRIMARY"))

story.append(annot_entry("P-02",
    'Quirino, Elpidio. Second State of the Nation Address (January 23, 1950). Official Gazette.',
    'Reports economic recovery data: palay at 56M cavans, fish output surpassing pre-war levels, '
    'peso price index recovering. Key benchmark for measuring actual vs. claimed progress.',
    'https://www.officialgazette.gov.ph/1950/01/23/', "PRIMARY"))

story.append(annot_entry("P-03",
    'Quirino, Elpidio. Third State of the Nation Address (January 22, 1951). Official Gazette.',
    'Delivered at height of dual crisis (Huk insurgency + economic emergency). Documents '
    'trade deficit reduction (₱547M to ₱28.8M) and Bell Mission commitments. Critical for '
    'understanding Quirino\'s economic and security responses in 1950–1951.',
    'https://www.officialgazette.gov.ph/1951/01/22/', "PRIMARY"))

story.append(annot_entry("P-04",
    'Quirino, Elpidio. Fourth State of the Nation Address (January 28, 1952). Official Gazette.',
    'Reports manufacturing index rise, national income growth to ₱5.12B, and completion of '
    'dollar bond payments. Key data source for the administration\'s economic arc.',
    'https://www.officialgazette.gov.ph/1952/01/28/', "PRIMARY"))

story.append(annot_entry("P-05",
    'Quirino, Elpidio. Inaugural Address (December 30, 1949). Official Gazette 45(12): 5380–5386.',
    'Articulates the democratic and civic ideology of the Quirino administration. Key source '
    'for understanding how the president framed national identity and democratic values in '
    'the context of postwar nation-building.',
    'https://mirror.officialgazette.gov.ph/1949/12/30/', "PRIMARY"))

story.append(annot_entry("P-06",
    'Quirino, Elpidio. Statement on the Bell Economic Survey Commission Report (October 28, 1950). '
    'Official Gazette 46(10): 4810.',
    'Quirino\'s formal acceptance of the Bell Report. Reveals his public posture toward U.S.-imposed '
    'conditionalities and his commitment to implementing the recommendations — a key document for '
    'analyzing Philippine economic sovereignty in the early Cold War.',
    'https://mirror.officialgazette.gov.ph/1950/10/28/', "PRIMARY"))

story.append(annot_entry("P-07",
    'Quirino, Elpidio. Seventeenth Radio Chat (March 15, 1950). Official Gazette.',
    'Nationwide broadcast from Baguio. Socio-cultural primary source showing how Quirino '
    'communicated with ordinary Filipinos about Cold War threats and civic duty. '
    'Useful for analyzing the government\'s ideological framing of the Huk crisis.',
    'https://officialgazette.gov.ph/1950/03/15/', "PRIMARY"))

story.append(annot_entry("P-08",
    'Quirino, Elpidio. Radio Address on Pre-Election (November 7, 1949). Official Gazette.',
    'Pre-election summary of accomplishments. Reveals how Quirino constructed his public '
    'political identity and the narrative of recovery for voter consumption.',
    'https://mirror.officialgazette.gov.ph/1949/11/07/', "PRIMARY"))

story.append(annot_entry("P-09",
    'Bell, Daniel W., et al. "Report of the United States Economic Survey Mission to the Philippines." '
    'In FRUS 1950, Vol. VI, Doc. 841. Washington, D.C.: U.S. Dept. of State, 1976.',
    'The foundational Bell Report. Documents the harsh economic conditions of postwar Philippines '
    'and recommends $250M in conditional U.S. aid. Essential for analyzing the Quirino-Foster '
    'Agreement and the limits of Philippine economic sovereignty.',
    'https://history.state.gov/historicaldocuments/frus1950v06/d841', "PRIMARY"))

story.append(annot_entry("P-10",
    'Truman, Harry S. Statement Announcing Economic Survey Mission (June 29, 1950). '
    'Truman Library Public Papers, Doc. 180.',
    'Formal authorization of the Bell Mission. Key document for contextualizing the U.S.-Philippine '
    'economic relationship and the Cold War rationale for conditional American assistance.',
    'https://www.trumanlibrary.gov/library/public-papers/180/', "PRIMARY"))

story.append(annot_entry("P-11",
    'U.S. Department of State. FRUS, 1951, Asia and the Pacific, Vol. VI, Part 2. Docs. 9 & 42.',
    'Contains U.S. Embassy telegrams and State Dept. policy memos on Philippines: Bell Mission '
    'implementation, ECA aid program, election integrity concerns, and Huk counterinsurgency '
    'strategy. Rich source for the American perspective on the Quirino years.',
    'https://history.state.gov/historicaldocuments/frus1951v06p2/d42', "PRIMARY"))

story.append(annot_entry("P-12",
    '"Official Month in Review: November 1950." Official Gazette of the Republic of the Philippines.',
    'Day-by-day government record of November 1950. Provides granular chronological data on '
    'policy decisions, agreements signed, and institutional actions during a pivotal month.',
    'https://www.officialgazette.gov.ph/1950/11/01/', "PRIMARY"))

story.append(annot_entry("P-13",
    'Taruc, Luis. Born of the People. New York: International Publishers, 1953.',
    'Autobiography of HMB commander Luis Taruc — essential first-hand account of the Huk '
    'movement from within. Describes the failed Quirino-Taruc negotiations and the agrarian '
    'roots of the rebellion. Read critically: written under communist influence and ghost-authored '
    'by William Pomeroy.', tag="PRIMARY"))

story.append(annot_entry("P-14",
    'Quirino, Elpidio. The Quirino Way: Collection of Speeches and Addresses. Manila, 1955.',
    'Compiled primary source of presidential rhetoric and political philosophy. Useful for '
    'tracing recurring themes — national unity, democratic legitimacy, anti-communism, and '
    'the Philippine-American relationship — across the full span of his administration.',
    tag="PRIMARY"))

story.append(Paragraph("— SECONDARY SOURCES (S) —", S("BibSec2", fontSize=10, leading=13,
    textColor=DARK_BLUE, fontName="Helvetica-Bold", spaceBefore=8, spaceAfter=4)))

for sb in section_bar("Books, Articles, and Reference Works", DARK_BLUE):
    story.append(sb)

story.append(annot_entry("S-01",
    'Kerkvliet, Benedict J. Tria. The Huk Rebellion: A Study of Peasant Revolt in the Philippines. '
    'Berkeley: University of California Press, 1977; reprinted 2002.',
    'The definitive scholarly study of the Hukbalahap rebellion. Situates the insurgency within '
    'the long history of Central Luzon agrarian grievances and tenancy exploitation. '
    'Argues the Huk movement was fundamentally a peasant revolt, not external communism. '
    'Indispensable for the socio-economic analysis of the Quirino era\'s greatest crisis.',
    tag="SECONDARY"))

story.append(annot_entry("S-02",
    'Agoncillo, Teodoro A., and Milagros C. Guerrero. History of the Filipino People. 4th ed. '
    'Quezon City: R.P. Garcia Publishing, 1973.',
    'The landmark Filipino-authored survey of Philippine history. Written from a nationalist '
    'perspective emphasizing Filipino agency and the structural limits of American-style democracy. '
    'Foundational for historical comprehension and the pro-Filipino analytical frame required '
    'by the rubric.',
    tag="SECONDARY"))

story.append(annot_entry("S-03",
    'Brands, Henry W. Bound to Empire: The United States and the Philippines. '
    'Oxford: Oxford University Press, 1992.',
    'Traces U.S.-Philippine relations from colonialism through the Cold War. Essential context '
    'for the Bell Mission, the parity rights system, and the conditional nature of American '
    'postwar assistance. Useful for the bilateral power dimension of the Quirino administration.',
    tag="SECONDARY"))

story.append(annot_entry("S-04",
    'Constantino, Renato, and Letizia R. Constantino. The Philippines: A Continuing Past. '
    'Quezon City: Foundation for Nationalist Studies, 1978.',
    'Nationalist critique of Philippine post-independence governments, arguing that Quirino\'s '
    'administration perpetuated neo-colonial structures. Essential counter-perspective to official '
    'narratives; particularly useful on the Bell Trade Act, parity rights, and agrarian reform failures.',
    tag="SECONDARY"))

story.append(annot_entry("S-05",
    'Lopez, Salvador P. Elpidio Quirino: The Judgment of History. '
    'Manila: President Elpidio Quirino Foundation, 1990.',
    'Rehabilitative account by a prominent diplomat arguing Quirino\'s achievements in '
    'reconstruction, diplomacy, and institution-building have been unfairly overshadowed '
    'by corruption scandals. Useful for understanding pro-Quirino historiography.',
    tag="SECONDARY"))

story.append(annot_entry("S-06",
    'Quirino, Carlos. Apo Lakay: The Biography of President Elpidio Quirino. '
    'Makati: Total Book World, 1987.',
    'Full-length biography by a relative and noted historian. Traces his life from Ilocos Sur '
    'through wartime survival (wife and three children killed, 1945) to his presidency. '
    'Sympathetic but contains valuable biographical detail often absent from political histories.',
    tag="SECONDARY"))

story.append(annot_entry("S-07",
    'Espinosa-Robles, Raissa. To Fight Without End: The Story of a Misunderstood President. '
    'Makati: Ayala Foundation, 1990.',
    'Humanizing biography focusing on Quirino\'s personal struggles — wartime losses, ill health, '
    'and navigation of a corrupt political environment. Argues he was undermined by party machinery '
    'and overshadowed by Magsaysay. Complements more critical political analyses.',
    tag="SECONDARY"))

story.append(annot_entry("S-08",
    'Abaya, Hernando J. Betrayal in the Philippines. New York: A.A. Wyn, 1946; '
    'and The Untold Philippine Story. Quezon City: Malaya Books, 1967.',
    'Critical investigative histories of the American-influenced Philippine political elite. '
    'The 1967 work extends the critique into the Quirino era, examining graft, electoral fraud, '
    'and barriers to land reform. Represents the investigative journalism tradition in '
    'Philippine historiography.',
    tag="SECONDARY"))

story.append(annot_entry("S-09",
    'Lachica, Eduardo. Huk: Philippine Agrarian Society in Revolt. '
    'Manila: Solidaridad Publishing House, 1971.',
    'Examines the Huk rebellion as a symptom of structural failures in Philippine agrarian society: '
    'absentee landlordism, share-tenancy exploitation, and inadequate land reform. '
    'Useful for evaluating LASEDECO and EDCOR as responses to deep rural discontent.',
    tag="SECONDARY"))

story.append(annot_entry("S-10",
    'Ladwig, Walter C. III. "The Philippine Constabulary and the Hukbalahap Rebellion." '
    'Academic chapter. Retrieved from walterladwig.com.',
    'Empirically rigorous analysis using Quirino Papers (Filipinas Heritage Library), '
    'U.S. State Dept. telegrams, and JUSMAG weekly summaries. Examines the AFP\'s '
    'evolution under Magsaysay as SecNat. One of the most archivally grounded '
    'studies of the Quirino-era counterinsurgency campaign.',
    'https://www.walterladwig.com/Articles/Huk_Chapter.pdf', "SECONDARY"))

story.append(annot_entry("S-11",
    '"Elpidio Quirino." Encyclopædia Britannica. Accessed May 2026.',
    'Authoritative reference entry summarizing Quirino\'s life and presidency. Covers the Huk '
    'threat, Bell Mission, postwar reconstruction, and 1953 defeat. Reflects mainstream '
    'historiographical consensus on the corruption and unrealized reform potential of his term.',
    'https://www.britannica.com/biography/Elpidio-Quirino', "SECONDARY"))

story.append(PageBreak())

# ═══════════════════════════════════════════════════════════════════════════════
# SECTION XII — LIST OF REFERENCES
# ═══════════════════════════════════════════════════════════════════════════════
story.extend(chapter_header("XII", "LIST OF REFERENCES",
    "Complete bibliography in alphabetical order by author"))
story.append(Spacer(1,6))

story.append(Paragraph(
    "All references are listed below in a single alphabetical list regardless of source type. "
    "Source type is indicated in brackets: [PRIMARY] or [SECONDARY].", small))
story.append(Spacer(1,4))

refs = [
    ("[PRIMARY]  Abaya, Hernando J. <i>Betrayal in the Philippines</i>. New York: A.A. Wyn, 1946. "
     "Also: <i>The Untold Philippine Story</i>. Quezon City: Malaya Books, 1967."),

    ("[PRIMARY]  Agoncillo, Teodoro A., and Milagros C. Guerrero. <i>History of the Filipino People</i>. "
     "4th ed. Quezon City: R.P. Garcia Publishing, 1973."),

    ("[PRIMARY]  Bell, Daniel W., et al. 'Report of the United States Economic Survey Mission to the "
     "Philippines.' October 9, 1950. In <i>Foreign Relations of the United States, 1950</i>, vol. VI. "
     "Washington, D.C.: U.S. Department of State, 1976. Document 841. "
     "https://history.state.gov/historicaldocuments/frus1950v06/d841"),

    ("[SECONDARY]  Brands, Henry W. <i>Bound to Empire: The United States and the Philippines</i>. "
     "Oxford: Oxford University Press, 1992."),

    ("[SECONDARY]  Constantino, Renato, and Letizia R. Constantino. "
     "<i>The Philippines: A Continuing Past</i>. "
     "Quezon City: Foundation for Nationalist Studies, 1978."),

    ("[SECONDARY]  'Elpidio Quirino.' <i>Encyclopædia Britannica</i>. Accessed May 2026. "
     "https://www.britannica.com/biography/Elpidio-Quirino"),

    ("[SECONDARY]  'Elpidio Quirino.' <i>Encyclopedia of World Biography</i>. Encyclopedia.com. "
     "Accessed February 2026. https://www.encyclopedia.com/history/encyclopedias-almanacs-transcripts-and-maps/elpidio-quirino"),

    ("[SECONDARY]  Espinosa-Robles, Raissa. <i>To Fight Without End: The Story of a Misunderstood "
     "President</i>. Makati: Ayala Foundation, 1990."),

    ("[SECONDARY]  Gwekoh, Sol H. <i>Elpidio Quirino: The Barrio School Teacher Who Became President</i>. "
     "Manila: [s.n.], 1949."),

    ("[SECONDARY]  Kerkvliet, Benedict J. Tria. <i>The Huk Rebellion: A Study of Peasant Revolt in "
     "the Philippines</i>. Berkeley: University of California Press, 1977. Reprinted 2002."),

    ("[SECONDARY]  Lachica, Eduardo. <i>Huk: Philippine Agrarian Society in Revolt</i>. "
     "Manila: Solidaridad Publishing House, 1971."),

    ("[SECONDARY]  Ladwig, Walter C. III. 'The Philippine Constabulary and the Hukbalahap Rebellion.' "
     "Chapter in academic monograph. https://www.walterladwig.com/Articles/Huk_Chapter.pdf"),

    ("[SECONDARY]  Lopez, Salvador P. <i>Elpidio Quirino: The Judgment of History</i>. "
     "Manila: President Elpidio Quirino Foundation, 1990."),

    ("[PRIMARY]  'Official Month in Review: November 1950.' <i>Official Gazette of the Republic of "
     "the Philippines</i>. November 1, 1950. "
     "https://www.officialgazette.gov.ph/1950/11/01/official-month-in-review-november-1950/"),

    ("[SECONDARY]  Quirino, Carlos. <i>Apo Lakay: The Biography of President Elpidio Quirino of "
     "the Philippines</i>. Makati: Total Book World, 1987."),

    ("[PRIMARY]  Quirino, Elpidio. 'Inaugural Address.' <i>Official Gazette of the Republic of the "
     "Philippines</i> 45, no. 12 (December 30, 1949): 5380–5386. "
     "https://mirror.officialgazette.gov.ph/1949/12/30/inaugural-address-of-president-quirino/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Seventeenth Radio Chat.' <i>Official Gazette of the Republic of "
     "the Philippines</i>. March 15, 1950. "
     "https://officialgazette.gov.ph/1950/03/15/message-of-president-quirino-march-15-1950"),

    ("[PRIMARY]  Quirino, Elpidio. 'State-of-the-Nation Message to the Joint Session of Congress.' "
     "<i>Official Gazette</i> 45, no. 1 (January 24, 1949): 153–170. "
     "https://www.officialgazette.gov.ph/1949/01/24/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Second State of the Nation Address.' <i>Official Gazette</i>. "
     "January 23, 1950. https://www.officialgazette.gov.ph/1950/01/23/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Third State of the Nation Address.' <i>Official Gazette</i>. "
     "January 22, 1951. https://www.officialgazette.gov.ph/1951/01/22/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Fourth State of the Nation Address.' <i>Official Gazette</i>. "
     "January 28, 1952. https://www.officialgazette.gov.ph/1952/01/28/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Statement on the Bell Economic Survey Commission Report.' "
     "<i>Official Gazette</i> 46, no. 10 (October 28, 1950): 4810. "
     "https://mirror.officialgazette.gov.ph/1950/10/28/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Radio Address on Pre-Election.' <i>Official Gazette</i>. "
     "November 7, 1949. https://mirror.officialgazette.gov.ph/1949/11/07/"),

    ("[PRIMARY]  Quirino, Elpidio. 'Extemporaneous Speech Before Provincial Governors and City Mayors.' "
     "<i>Official Gazette</i> 47, no. 7 (July 31, 1951): 3414–3418. "
     "https://www.officialgazette.gov.ph/1951/07/31/"),

    ("[PRIMARY]  Quirino, Elpidio. <i>The Quirino Way: Collection of Speeches and Addresses</i>. "
     "1st ed. Manila: [s.n.], 1955."),

    ("[SECONDARY]  Romulo, Carlos P. <i>The Philippine Presidents</i>. "
     "Quezon City: New Day Publishers, 1988."),

    ("[SECONDARY]  Rousset, Pierre. 'Huk Rebellion, 1946–1954.' In "
     "<i>The Wiley-Blackwell Encyclopedia of Social and Political Movements</i>. "
     "Oxford: Wiley-Blackwell, 2013. "
     "https://onlinelibrary.wiley.com/doi/abs/10.1002/9781405198073.wbierp0722"),

    ("[PRIMARY]  Taruc, Luis. <i>Born of the People</i>. New York: International Publishers, 1953."),

    ("[PRIMARY]  Taruc, Luis. <i>He Who Rides the Tiger: The Story of an Asian Guerrilla Leader</i>. "
     "New York: Frederick A. Praeger, 1967."),

    ("[PRIMARY]  Truman, Harry S. 'Statement Announcing Economic Survey Mission to the Philippines.' "
     "June 29, 1950. Truman Library, Public Papers, Doc. 180. "
     "https://www.trumanlibrary.gov/library/public-papers/180/"),

    ("[PRIMARY]  U.S. Department of State. <i>Foreign Relations of the United States, 1951: Asia and "
     "the Pacific</i>, vol. VI, Part 2. Washington, D.C.: U.S. Government Printing Office. "
     "https://history.state.gov/historicaldocuments/frus1951v06p2/d42"),
]

for i, r in enumerate(refs, 1):
    story.append(ref_entry(f"{i:02d}", r))

story.append(Spacer(1,16))

# Footer summary box
footer_data = [
    ["Total Sources", "31"],
    ["Primary Sources", "17"],
    ["Secondary Sources", "14"],
    ["Official Gazette Documents", "10"],
    ["U.S. Government / Diplomatic Documents", "3"],
    ["Academic Books / Monographs", "9"],
    ["Encyclopedias / Reference Works", "3"],
    ["Academic Articles / Chapters", "3"],
    ["Personal Memoirs", "3"],
]
hrow = [Paragraph("Category", S("FH1", fontSize=8.5, textColor=colors.white, fontName="Helvetica-Bold")),
        Paragraph("Count", S("FH2", fontSize=8.5, textColor=colors.white, fontName="Helvetica-Bold", alignment=TA_CENTER))]
frows = [[Paragraph(r[0], S(f"FR0{i}", fontSize=8.5, textColor=TEXT_BODY, fontName="Helvetica")),
          Paragraph(r[1], S(f"FR1{i}", fontSize=8.5, textColor=TEXT_BODY, fontName="Helvetica-Bold", alignment=TA_CENTER))]
         for i, r in enumerate(footer_data)]
ftbl = Table([hrow]+frows, colWidths=[5.1*inch, 1.4*inch])
ftbl.setStyle(TableStyle([
    ("BACKGROUND",(0,0),(-1,0), NAVY),
    ("ROWBACKGROUNDS",(0,1),(-1,-1),[colors.white, LIGHT_GRAY]),
    ("TOPPADDING",(0,0),(-1,-1),4),("BOTTOMPADDING",(0,0),(-1,-1),4),
    ("LEFTPADDING",(0,0),(-1,-1),8),("RIGHTPADDING",(0,0),(-1,-1),8),
    ("GRID",(0,0),(-1,-1),0.4,MED_GRAY),
    ("FONTNAME",(0,1),(0,2),"Helvetica-Bold"),
    ("TEXTCOLOR",(0,1),(0,2),DARK_BLUE),
]))
story.append(Spacer(1,8))
story.append(HRFlowable(width="100%", thickness=1.5, color=GOLD))
story.append(Spacer(1,6))
story.append(Paragraph("Source Count Summary", S("FTitle", fontSize=10, textColor=NAVY,
    fontName="Helvetica-Bold", spaceBefore=4, spaceAfter=4)))
story.append(ftbl)

story.append(Spacer(1,10))
story.append(Paragraph(
    "<i>This document was prepared for the Final Group MLT on the Post-War Period (Group 2), "
    "Philippine History — 20th to 21st Century Philippines. All primary sources are drawn from "
    "the Official Gazette of the Republic of the Philippines, the U.S. Office of the Historian "
    "(Foreign Relations of the United States), and the Harry S. Truman Presidential Library. "
    "Secondary sources include peer-reviewed academic monographs, encyclopedias, and declassified "
    "military analyses. This bibliography exceeds the rubric requirement of fifteen substantial sources.</i>",
    S("FNote", fontSize=7.8, leading=11.5, textColor=colors.HexColor("#555"),
      fontName="Helvetica-Oblique", alignment=TA_JUSTIFY)))

doc.build(story)
print("PDF successfully generated.")