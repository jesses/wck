package Box2DAS.Common {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	
	/**
	 * Holds static Box2D definition objects that can be reused for performance.
	 */
	public class b2Def {
			
		/// Body, shape, fixture & joint defs that can be reused for performance.
		public static var body:b2BodyDef;
		public static var fixture:b2FixtureDef;
		public static var polygon:b2PolygonShape;
		public static var circle:b2CircleShape;
		public static var distanceJoint:b2DistanceJointDef;
		public static var gearJoint:b2GearJointDef;
		public static var lineJoint:b2LineJointDef;
		public static var mouseJoint:b2MouseJointDef;
		public static var prismaticJoint:b2PrismaticJointDef;
		public static var pulleyJoint:b2PulleyJointDef;
		public static var revoluteJoint:b2RevoluteJointDef;
		public static var weldJoint:b2WeldJointDef;
		public static var frictionJoint:b2FrictionJointDef;
		
		/**
		 * Create static definition objects.
		 */
		public static function initialize():void {
			body = new b2BodyDef();
			fixture = new b2FixtureDef();
			polygon = new b2PolygonShape();
			circle = new b2CircleShape();
			distanceJoint = new b2DistanceJointDef();
			gearJoint = new b2GearJointDef();
			lineJoint = new b2LineJointDef();
			mouseJoint = new b2MouseJointDef();
			prismaticJoint = new b2PrismaticJointDef();
			pulleyJoint = new b2PulleyJointDef();
			revoluteJoint = new b2RevoluteJointDef();
			weldJoint = new b2WeldJointDef();
			frictionJoint = new b2FrictionJointDef();
		}
	}
}