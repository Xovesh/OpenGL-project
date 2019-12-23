output = open("desierto_result.obj","w")
input  = open("desierto.obj","r")

for i in input:
    if(i[0] == 'f'):
        output.write("f ")
        aux = i[2:].split(" ")
        for elem in aux:
            output.write(elem.split("/")[0] + " ")
        output.write("\n")
    else:
        output.write(i)
