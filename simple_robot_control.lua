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
      publisher1=simROS.advertise('/simulationTime','std_msgs/Float32')
      publisher2=simROS.advertise('/pose','geometry_msgs/Pose')
      subscriber1=simROS.subscribe('/cmd_vel','geometry_msgs/Twist','subscriber_cmd_vel_callback')
   end
end


function sysCall_actuation()
   -- Send an updated simulation time message, and send the transform of the object attached to this script:
   if rosInterfacePresent then
      -- publish time and pose topics
      simROS.publish(publisher1,{data=sim.getSimulationTime()})
      simROS.publish(publisher2,getPose("Chassis"))
      -- send a TF
      simROS.sendTransform(getTransformStamped(objectHandle,objectName,-1,'world'))
      -- To send several transforms at once, use simROS.sendTransforms instead
   end
end
 
function sysCall_cleanup()
    -- Following not really needed in a simulation script (i.e. automatically shut down at simulation end):
    if rosInterfacePresent then
        simROS.shutdownPublisher(publisher1)
        simROS.shutdownPublisher(publisher2)
        simROS.shutdownSubscriber(subscriber1)
    end
end
 
function subscriber_cmd_vel_callback(msg)
   spdLin = msg["linear"]["x"]
   spdAng = msg["angular"]["z"]
   kLin = -0.5
   kAng = -0.2
   spdRoue = kLin*spdLin
   spdAngle = kAng*spdAng
   spdGauche = (spdRoue - spdAngle)
   spdDroite = (spdRoue + spdAngle)
   --sim.addStatusbarMessage('vitesseroue'..spdRoue)
   --sim.addStatusbarMessage('vitesseangle'..spdAngle)
   --sim.addStatusbarMessage('vitessegauche'..spdGauche)
   --sim.addStatusbarMessage('vitessedroite'..spdDroite)
   sim.setJointTargetVelocity(MoteurArriereGauche,spdGauche)
   sim.setJointTargetVelocity(MoteurAvantGauche,spdGauche)
   sim.setJointTargetVelocity(MoteurArriereDroit,spdDroite) 
   sim.setJointTargetVelocity(MoteurAvantDroit,spdDroite) 
end
 
function getPose(objectName)
   -- This function get the object pose at ROS format geometry_msgs/Pose
   objectHandle=sim.getObjectHandle(objectName)
   relTo = -1
   p=sim.getObjectPosition(objectHandle,relTo)
   o=sim.getObjectQuaternion(objectHandle,relTo)
   return {
      position={x=p[1],y=p[2],z=p[3]},
      orientation={x=o[1],y=o[2],z=o[3],w=o[4]}
   }
end
 
function getTransformStamped(objHandle,name,relTo,relToName)
   -- This function retrieves the stamped transform for a specific object
   t=sim.getSystemTime()
   p=sim.getObjectPosition(objHandle,relTo)
   o=sim.getObjectQuaternion(objHandle,relTo)
   return {
      header={
	 stamp=t,
	 frame_id=relToName
      },
      child_frame_id=name,
      transform={
	 translation={x=p[1],y=p[2],z=p[3]},
	 rotation={x=o[1],y=o[2],z=o[3],w=o[4]}
      }
   }
end


function sysCall_dynCallback(inData)
  -- This function simulate the behavior of the robot in the water
   objectName="Chassis"
   warthog=sim.getObjectHandle(objectName)
   Vit, W =sim.getObjectVelocity(warthog,-1)
   -- sim.handleDynamics(0.1)
   p=sim.getObjectPosition(warthog,-1)
   -- if x> 25
   x=p[1]
   y=p[2]
   z=p[3]
   po = 100000
   g= 9.8
   l = 1
   -- m=5
   m=sim.getShapeMassAndInertia(warthog)
   Cx = 1.05
   -- print("in water")
   -- print("profondeur est de ", z)
   F =  { 1/2*(po*Vit[1]*math.abs(Vit[1])*l*l*Cx/m), 1/2*(po*Vit[2]*math.abs(Vit[2])*l*l*Cx/m),-(po*g*l*l*math.min(0, (math.max(math.min(z,0),-l)))/m) - 1/2*(po*Vit[3]*math.abs(Vit[3])*l*l*Cx/m)  }
   print(F)
      -- F  = {0,0,0} 
      -- + 1/2*(p*Vit[3]*math.abs(Vit[3])*l*l*Cx/m)
   sim.addForce(warthog,{0,0,0},F)
-- sim.resetDynamicObject(warthog)
    if x > 25 then 
       F = {0,0, -(po*g*l*l*max(0, (l+min(z,0)+z)/m)) }
       sim.addForce(objectHandle, p, F)
    end
end

