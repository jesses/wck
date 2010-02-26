package gravity {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	import wck.*;
	import gravity.*;
	import misc.*;
	import flash.utils.*;
	import flash.events.*;
	import flash.display.*;
	import flash.text.*;
	import flash.geom.*;
	import flash.ui.*;

	/**
	 * Provides capsule, or rounded rectangle gravity.
	 */
	public class GravityCapsule extends GravityRadial {
		
		public var capsule:V2 = new V2(50, 50);
		
		/**
		 * Get the rectangular dimentions that define the capsule.
		 */
		public override function initStep():void {
			var b:Rectangle = Util.bounds(this);
			capsule.xy(b.width / 2, b.height / 2);
		}
		
		/**
		 * Calculate the capsule gravity vector from the Box2d world point.
		 */
		public override function gravity(p:V2, b:b2Body = null, b2:BodyShape = null):V2 {
			/// Transform the point into the local coordinate space.
			var localP:Point = Util.localizePoint(this, world, V2.multiplyN(p, world.scale).toP());
			/// If the point is within the minimum region, just return base gravity.
			if(Math.abs(localP.x) < capsule.x && Math.abs(localP.y) < capsule.y) {
				origin = V2.fromP(Util.localizePoint(world, this, new Point(localP.x, localP.y + 10))).divideN(world.scale);
			}
			else {
				/// Find the closest point on the rectangle.
				var rectP:Point = new Point(Math.min(capsule.x, Math.abs(localP.x)), Math.min(capsule.y, Math.abs(localP.y)));
				rectP.x *= (localP.x > 1 ? 1 : -1);
				rectP.y *= (localP.y > 1 ? 1 : -1);
				/// Convert that point into the b2 world coordinate space.
				origin = V2.fromP(Util.localizePoint(world, this, rectP)).divideN(world.scale);
			}
			/// Calculate radial gravity from that point.
			return super.gravity(p, b, b2);
		}

	}
}