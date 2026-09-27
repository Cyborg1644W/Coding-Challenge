import pyautogui
import pyperclip
import time

MESSAGE = "Hello, this is an automated message!"
REPEAT = 100
DELAY_BETWEEN = 0.03    

print("Starting in 3 seconds... switch to your target window!")
time.sleep(3)

for i in range(REPEAT):
    pyperclip.copy(MESSAGE)           # copy to clipboard
    pyautogui.hotkey('command', 'v')  # paste (cmd+v on Mac)
    pyautogui.press('enter')
    print(f"Sent message {i+1}/{REPEAT}")
    time.sleep(DELAY_BETWEEN)

print("Done.")