print("Content-Type: text/html\r\n\r\n")
#!/usr/bin/env python3
print("<pre style=\"color : red; background: black; padding: 20px; text-align: center\">")
def sierpinski(n):
    d = ["x"]
    for i in range(n):
        sp = " " * (2 ** i)
        d = [sp+x+sp for x in d] + [x+" "+x for x in d]
    return d
 
print("\n".join(sierpinski(6)))
print("</pre>")