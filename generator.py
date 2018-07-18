MAIN = 0
QUADCOPTER = 1 


def updatetestmain(testmainfile,botnum,indents):
    lineno = 0
    f = open(testmainfile,"r")
    s = ""
    first = True
    for line in f.readlines():
        if "selectedRobot" in line and first:
            s += indents*" "+"private static int selectedRobot = "+ str(botnum)+";\n"
            first = False 
        else:
            s+= line 
    f.close() 
    f = open(testmainfile,"w")
    f.write(s)
    f.close() 

def updatebotinfo(botinfofile,botnum,botip,indents):
    lineno = 0
    f = open(botinfofile,"r")
    s = ""
    first = True
    second = False
    skip = False
    ipline = indents*" "+ 'ip = "'+botip + '";';
    for line in f.readlines():
        if skip:
           s+= ipline
           skip = False 
           continue

        if "NEXUS7" in line and first and botnum == 0:
           s += line
           first = False
           second = True
           skip = True

        elif "NEXUS7" in line and second and botnum == 1:
           s += line
           second = False
           skip = True
        else:
            s+= line 

    f.close() 
    f = open(botinfofile,"w")
    f.write(s)
    f.close() 
       

updatetestmain("testmainfile",8,8)
updatebotinfo("botinfofile",0,"testup",4)
