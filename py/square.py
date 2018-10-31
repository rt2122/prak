def sq(n):
    def print_hor():
        print("+ " + "- " * n + "+ " + "- " * n + "+")
    def print_vert():
        print(("/ " + "  " * n + "/ " + "  " * n + "/\n") * n, end = "")
    print_hor()
    print_vert()
    print_hor()
    print_vert()
    print_hor()
    
sq(2)