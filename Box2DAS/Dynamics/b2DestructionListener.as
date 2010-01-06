package Box2DAS.Dynamics {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	import flash.events.*;
	import flash.utils.*;
	
	/// Joints and fixtures are destroyed when their associated
	/// body is destroyed. Implement this listener so that you
	/// may nullify references to these joints and shapes.
	///
	/// Default AS3 Implementation: If the joint/fixture userData is an event dispatcher (like
	/// a display object) then an AS3 native event will be dispatched off of that object.
	///
	public class b2DestructionListener {

		/// Called when any joint is about to be destroyed due
		/// to the destruction of one of its attached bodies.		
		public function SayGoodbyeJoint(j:b2Joint):void {
			var ed:IEventDispatcher = j.m_userData as IEventDispatcher;
			if(ed) {
				ed.dispatchEvent(new GoodbyeJointEvent(j));
			}
		}
		
		/// Called when any fixture is about to be destroyed due
		/// to the destruction of its parent body.		
		public function SayGoodbyeFixture(f:b2Fixture):void {
			var ed:IEventDispatcher = f.m_userData as IEventDispatcher;
			if(ed) {
				ed.dispatchEvent(new GoodbyeFixtureEvent(f));
			}
		}
	}
}