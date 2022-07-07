import numpy
import pandas as pd
import datetime
import matplotlib.pyplot as plt



now_time=datetime.datetime.now()
data=pd.read_csv("wavedata_save07070906.csv")
#st_time=data.datetime.iat[-1]
inner=data.inner[0:12115].mean()
motorate=data.moto[0:12115].mean()
outer=data.outer[0:12115].mean()

innerdis=inner*60*4*38*3.1415/1000
innermotodis=motorate*60*4*149.9*3.1415/1000
outerdis=outer*60*4*38*3.1415/1000
outermotodis=motorate*60*4*197*3.1415/1000

print(innerdis)
print(innermotodis)
print(outerdis)
print(outermotodis)
innerpercent=(innermotodis-innerdis)/innermotodis
outerpercent=(outermotodis-outerdis)/outermotodis
#data=[innerdis,innermotodis]

# print(innerpercent)
# print(outerpercent)
fig = plt.figure(figsize=(10,4))
ax1 = fig.add_subplot(121)
size=numpy.array([innermotodis,innerdis])
def absolute_value(val):
    a  = numpy.round(val/100.*size.sum(), 6)
    return a
colors=['#E0FF5A','#FFEB00']
labels=['Moto inner 2H total distance','Inner wheel 2H total distance']
ax1.pie(size,colors=colors,labels=labels,radius=1,labeldistance=1.1,autopct=absolute_value,explode=[0.01, 0.05])
plt.title("Inner wheel distance percent: %.4f%%"%(innerpercent*100))

ax2=fig.add_subplot(122)
size2=numpy.array([outermotodis,outerdis])
def absolute_value1(val):
    a  = numpy.round(val/100.*size2.sum(), 6)
    return a
colors1=['#00FFFF','#00EBCD']
labels1=['Moto outer 2H total distance','Outer wheel 2H total distance']
ax2.pie(size2,colors=colors1,labels=labels1,radius=1,labeldistance=1.1,explode=[0.01, 0.05],autopct=absolute_value1)   #autopct='%.2f%%'
plt.title("Outer wheel distance percent: %.4f%%"%(outerpercent*100))
plt.show()
#print(st_time)
#print ((st_time+datetime.timedelta(hours=-1)).strftime("%Y-%m-%d %H:%M:%S"))