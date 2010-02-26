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
	import misc.*;
	import flash.utils.*;
	import flash.events.*;
	import flash.display.*;
	import flash.text.*;
	import flash.geom.*;
	import flash.ui.*;
	
	/**
	 * Base class for gravity objects that can be dropped into a world. Gravity will be
	 * effected by its display object transformation.
	 * TO DO: Currently only one custom gravity object can be applied to a world. It may
	 * be useful to use more than one in various regions. How to define regions though? Sensors?
	 * category / mask data?
	 */
	public class Gravity extends Entity {

		public var base:V2;
		public var world:World;
		
		public override function create():void {
			visible = false;
			world = Util.findAncestorOfClass(this, World) as World;
			world.ensureCreated();
			world.customGravity = this;
			base = new V2(world.gravityX, world.gravityY);
		}
		
		/**
		 *
		 */
		public function initStep():void {
		}
		
		/**
		 *
		 */
		public function gravity(p:V2, b:b2Body = null, b2:BodyShape = null):V2 {
			return base;
		}
	}
}