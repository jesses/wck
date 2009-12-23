package {
	
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
	
	public class BoxMan extends Box {
		
		public var contacts:ContactList;
		
		public override function create():void {
			reportBeginContact = true;
			reportEndContact = true;
			fixedRotation = true;
			super.create();
			listenWhileVisible(world, World.TIME_STEP, parseInput, false, 10);
			listenWhileVisible(this, ContactEvent.BEGIN_CONTACT, handleContact);
			contacts = new ContactList();
			contacts.listenTo(this);
		}
		
		public function handleContact(e:ContactEvent):void {
			var p:Pellet = e.other.m_userData as Pellet;
			if(p) {
				Util.addChildAtPosOf(world, new FX1(), p);  
				p.remove();
			}
		}
		
		public function parseInput(e:Event):void {
			var manifold:b2WorldManifold = null;
			var dot:Number = -1;
			if(!contacts.isEmpty()) {
				contacts.forEach(function(keys:Array, c:ContactEvent) {
					var wm:b2WorldManifold = c.getWorldManifold();
					if(wm.m_normal) { 
						var d:Number = wm.m_normal.dot(gravity);
						if(!manifold || d > dot) {
							manifold = wm;
							dot = d;
						}
					}
				});
				contacts.clean();
			}
			var left:Boolean = Input.kd('A', 'LEFT');
			var right:Boolean = Input.kd('D', 'RIGHT');
			var jump:Boolean = Input.kp(' ', 'UP');
			var v:V2;
			if(jump && manifold) {
				v = manifold.m_normal.clone().multiplyN(-2);
				b2body.ApplyImpulse(v, b2body.GetWorldCenter());
			}
			else if(left) {
				b2body.ApplyImpulse(new V2(-0.1, 0), b2body.GetWorldCenter());
			}
			else if(right) {
				b2body.ApplyImpulse(new V2(0.1, 0), b2body.GetWorldCenter());				
			}
		}
	}
}

/*

dot = wm.m_normal.dot(down);
					if(dot >= groundDot) {
						groundNormal = wm.m_normal.clone();
						groundDot = dot;
					}

*/
