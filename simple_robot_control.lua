
function sysCall_init()
   -- The child script initialization
   -- sim.addStatusbarMessage('sysCall_init')
   objectName="Chassis"
   objectHandle=sim.getObjectHandle(objectName)
   -- get left and right motors handles
   MoteurArriereGauche = sim.getObjectHandle("MoteurArriereGauche")
   MoteurArriereDroit = sim.getObjectHandle("MoteurArriereDroit")
   MoteurAvantGauche = sim.getObjectHandle("MoteurAvantGauche")
   MoteurAvantDroit = sim.getObjectHandle("MoteurAvantDroit")
   rosInterfacePresent=simROS
   -- Prepare the publishers and subscribers :
   if rosInterfacePresent then


   end
end
