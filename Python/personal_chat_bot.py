import os
import sys
import time  # We need this for the typing delay
from groq import Groq

# 1. Initialize the Groq client
try:
    client = Groq(
        api_key="API_KEY" # Put your actual key here
    )
except Exception as e:
    print(f"Error initializing Groq client: {e}")
    sys.exit(1)

# 2. Define the butler's persona
BUTLER_PERSONA = (
    "You are Jarvis, a highly sophisticated, loyal, and articulate personal butler. "
    "Your tone should be impeccably polite, formal, yet warm and deeply helpful. "
    "Address the user as 'Sir' or 'Madame' (or keep it neutral unless specified). "
    "Anticipate needs, offer structured assistance, and maintain an air of upper-class elegance "
    "while answering questions or executing requests. "
    "Do NOT use any markdown formatting, bolding, or asterisks in your responses."
)

# 3. Custom function for the typing effect
def type_text(text, delay=0.025):
    """Prints text character by character and removes asterisks."""
    # Strip out any asterisks the LLM tries to sneak in
    clean_text = text.replace('*', '')
    
    # Print the name instantly, but stay on the same line
    print("\nJarvis: ", end="")
    
    # Type out the rest character by character
    for char in clean_text:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(delay)
    
    # Drop down a line when finished
    print("\n")

def main():
    # 4. Initialize conversation history
    messages = [
        {"role": "system", "content": BUTLER_PERSONA}
    ]
    
    print("====================================================")
    print("   Your Personal Butler is online and at your service.   ")
    print("   Type 'exit' or 'quit' to dismiss him.            ")
    print("====================================================\n")
    
    # Use the typing effect for the greeting too
    type_text("Good day. How may I be of assistance to you today?")

    # 5. Main chat loop
    while True:
        try:
            # Get user input
            user_input = input("You: ").strip()
            
            # Check for exit commands
            if user_input.lower() in ['exit', 'quit']:
                type_text("Very well. Have a splendid rest of your day.")
                break
                
            if not user_input:
                continue

            # Append user message to history
            messages.append({"role": "user", "content": user_input})

            # 6. Call the Groq API (using the current 3.1 model)
            completion = client.chat.completions.create(
                model="llama-3.1-8b-instant", 
                messages=messages,
                temperature=0.6,
                max_tokens=1024,
                top_p=1,
                stream=False,
            )

            # Extract butler response
            butler_response = completion.choices[0].message.content
            
            # Print response using our new typing function
            type_text(butler_response)
            
            # Save it to history
            messages.append({"role": "assistant", "content": butler_response})

        except KeyboardInterrupt:
            # Handle Ctrl+C gracefully
            type_text("Apologies for the abrupt ending. I am always here should you require me.")
            break
        except Exception as e:
            print(f"\nAn error occurred: {e}\n")

if __name__ == "__main__":
    main()
