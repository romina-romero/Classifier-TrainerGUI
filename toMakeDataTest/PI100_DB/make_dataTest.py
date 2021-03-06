import os, sys, shutil, random

classes =[]
classes.append(("bolso",["briefcase", "luggage", "tote"],["train_train","tvmonitor_train"]))
classes.append(("envase",[ "can", "goblet", "mug","nutrition"],["train_train","bicycle_train"]))
classes.append(("comida", ["baked_goods","cheese","chocolate"],["train_train","bicycle_train"]))
classes.append(("ruedas", ["baby_carriage","bike","electric_car"],["tvmonitor_train","diningtable_train"]))
classes.append(("electronico", ["calculator","camcorder","car_GPS","cellphone","computer_keyboard","computer_monitor","computer_mouse","drill","eletectric_fan","handheld_GPS","lens","microwave_oven","telephone"],["bicycle_train","diningtable_train"]))

os.mkdir("dataset")
for superclass, cls, negs in classes:
	fnegs = []
	for neg in negs:
		nl = os.listdir(neg)
		for f in nl:
			fnegs.append(neg+os.sep+f)
	for cl in cls:
		os.mkdir("dataset"+os.sep+cl)
		os.mkdir("dataset"+os.sep+cl+os.sep+"positive")
		os.mkdir("dataset"+os.sep+cl+os.sep+"negative")
		cf = os.listdir(superclass)
		copyfirst = True
		for fl in cf:
			if fl.startswith(cl):
				shutil.copy2(superclass+os.sep+fl,"dataset"+os.sep+cl+os.sep+"positive"+os.sep+fl)
				if copyfirst:
					shutil.copy2(superclass+os.sep+fl,"dataset"+os.sep+cl)
					copyfirst = False
		for i in range(50):
			shutil.copy2(random.choice(fnegs),"dataset"+os.sep+cl+os.sep+"negative"+os.sep+str(i)+".jpg") 


