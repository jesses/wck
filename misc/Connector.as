package misc {

	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;
	import flash.ui.*;
	import misc.*;
	
	/**
	 * Base display object that visually "connects" two other display objects, whereever they may
	 * be in the display object hierarchy. Subclasses must override and implement "render" to draw
	 * the connection.
	 */
	public class Connector extends Entity {
		
		public var from:DisplayObject; // connect from this object...
		public var to:DisplayObject; // ... to this object.
		public var localPoint1:Point = new Point(0, 0); // Local point in object 1.
		public var localPoint2:Point = new Point(0, 0); // Local point in object 2.
		public var point1:Point = new Point(0, 0); // The connector should be set to this position within it's parent.
		public var point2:Point = new Point(0, 0); // The destination position (within the parent)
		public var length:Number; // The length of the connection. The Connector should draw from zero to this far down the x axis.
		
		/**
		 * Listen to a stage for enter frame events at a really low priority - the listener should fire
		 * after everything else is moved around.
		 */
		public override function create():void {
			listenWhileVisible(stage, Event.ENTER_FRAME, handleEnterFrame, false, -1000);
		}
		
		/**
		 * Positon and rotate the connector to set it up for rendering.
		 */
		public function handleEnterFrame(e:Event):void {
			if(from) {
				point1 = Util.localizePoint(parent, from, localPoint1);
			}
			if(to) {
				point2 = Util.localizePoint(parent, to, localPoint2);
			}
			setPos(point1);
			var dx:Number = point2.x - x;
			var dy:Number = point2.y - y;
			length = Math.sqrt(dx * dx + dy * dy);
			rotation = Math.atan2(dy, dx) * Util.R2D;
			render();
		}
		
		/**
		 * Override this to actually render the connector. Simply draw out from (0,0) to (length, 0).
		 */
		public function render():void {
		}
	}
}