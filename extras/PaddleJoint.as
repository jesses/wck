package extras {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	import wck.*;
	import shapes.*;
	import misc.*;
	import extras.*;
	import flash.utils.*;
	import flash.events.*;
	import flash.display.*;
	import flash.text.*;
	import flash.geom.*;
	
	/**
	 *
	 */
	public class PaddleJoint extends Joint {
		
		[Inspectable(defaultValue="left",enumeration="left,right")]
		public var leftOrRight:String = 'left';
		
		public override function create():void {
			type = 'Revolute';
			motorStrength = 500; 
			if(leftOrRight == 'left') {
				upperLimit = 0;
				lowerLimit = -1;
			}
			else {
				upperLimit = 1;
				lowerLimit = 0;
			}
			enableLimit = true;
			enableMotor = true;
			super.create();
			listenWhileVisible(world, World.TIME_STEP, parseInput, false, 1);
		}
		
		public function parseInput(e:Event):void {
			b2joint.WakeUp();
			var mult:int;
			if(leftOrRight == 'left') {
				mult = (Input.keysDown[KeyCodes.LEFT] || Input.keysDown[KeyCodes.Z]) ? -1 : 1;
			}
			else {
				mult = (Input.keysDown[KeyCodes.RIGHT] || Input.keysDown[KeyCodes.FSLASH]) ? 1 : -1;
			}
			(b2joint as b2RevoluteJoint).m_motorSpeed = 100 * mult;
		}
	}
}