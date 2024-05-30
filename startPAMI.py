import sys
import os
def print_color(color):
    if color == "YELLOW":
        print("Yellow color")
    elif color == "BLUE":
        print("Blue color")
    else:
        print("Unknown color")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python program.py <color>")
        sys.exit(1)

    color = sys.argv[1]
    print_color(color)
    exe_path = os.path.dirname(os.path.abspath(__file__))
    python_script_path = os.path.join(exe_path, "UDPServer.py")
    os.system("python3 "+str(python_script_path)+(" 0" if color == "BLUE" else " 1"))
