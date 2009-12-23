package {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	
	import flash.display.*;
	import flash.events.*;
	
	public class Hello extends MovieClip {
		
		public var world:b2World;
		public var scale:Number = 30;
		public var box:b2Body;
		public var circle:b2Body;
		public var lastBody:b2Body;
		
		public function Hello() {
			b2Base.initialize();
			world = new b2World(new V2(0, 10));
			world.debug = new Shape();
			addChild(world.debug);
			world.debugScale = scale;
			var body:b2Body;
			b2Def.circle.m_radius = 5 / scale;
			b2Def.fixture.shape = b2Def.circle;
			//b2Def.fixture.restitution = 0.5;
			b2Def.fixture.density = 1.0;
			//b2Def.fixture.filter.groupIndex = 1;
			//b2Def.fixture.filter.categoryBits = 0x0002;
			//b2Def.fixture.filter.maskBits = 0x0004;
			var ud:Shape;
			for(var d:uint = 0; d <= 2; d += 2) {
				b2Def.body.type = d;
				for(var i:int = 0; i < 200; ++i) {
					b2Def.body.position.x = Math.random() * 550 / scale;
					b2Def.body.position.y = (Math.random() * 400 - 200 * d) / scale;
					lastBody = new b2Body(world, b2Def.body);
					new b2Fixture(lastBody, b2Def.fixture);
				}
			}
			//b2Def.fixture.filter.categoryBits = 0x0004;
			//b2Def.fixture.filter.maskBits = 0x0002;
			b2Def.body.type = 1;
			b2Def.body.position.x = 200 / scale;
			b2Def.body.position.y = 400 / scale;
			b2Def.body.allowSleep = false;
			b2Def.circle.m_radius = 25 / scale;
			circle = new b2Body(world, b2Def.body);
			new b2Fixture(circle, b2Def.fixture);
			b2Def.body.position.x = 400 / scale;
			b2Def.polygon.SetAsBox(25 / scale, 25 / scale, new V2(0, -100 / scale), Math.PI / 4);
			b2Def.fixture.shape = b2Def.polygon;
			box = new b2Body(world, b2Def.body);
			new b2Fixture(box, b2Def.fixture);
			stage.addEventListener(Event.ENTER_FRAME, handleEnterFrame);
			stage.addEventListener(MouseEvent.MOUSE_DOWN, handleMouseDown);
		}
		
		public function handleMouseDown(e:MouseEvent):void {
			var aabb:AABB = new AABB();
			aabb.lowerBound.x = mouseX / scale - .01;
			aabb.lowerBound.y = mouseY / scale - .01;
			aabb.upperBound.x = mouseX / scale + .01;
			aabb.upperBound.y = mouseY / scale + .01;
			world.QueryAABB(function(f:b2Fixture):Boolean { 
				trace('clicked on: ', f.m_shape);
				return true;
			}, aabb);
		}
	
		public function handleEnterFrame(e:Event):void {
			world.Step(1 / 30, 15, 15);
			box.SetLinearVelocity(new V2(0, -2));
			box.SetAngularVelocity(.5);
			circle.SetLinearVelocity(new V2(0, -2));
			var m:V2 = new V2(mouseX / scale, mouseY / scale);
			//for (var c:b2Contact = world.GetContactList(); c; c = c.GetNext()) {
			//	trace('contact:', c.m_fixtureA, c.m_fixtureB, c.GetNext());
			//}
			if(box.m_fixtureList.TestPoint(m)) {
				trace('inside box!');
			}
			if(circle.m_fixtureList.TestPoint(m)) {
				trace('inside circle!');
			}
			world.RayCast(function(f:b2Fixture, point:V2, normal:V2, fraction:Number):int {
				trace('bottom stage: ', f.m_shape, point, normal, fraction);
				return fraction;
			}, new V2(0, 500 / scale), new V2(550 / scale, 500 / scale));
		}
	}
}