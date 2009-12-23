package misc {
	
	import flash.display.*;
	import flash.utils.*;
	import flash.geom.*;
	
	/**
	 * Connector that uses a mask to display a pre-drawn "line" only to the length between the 
	 * two objects.
	 */
	public class ConnectorMask extends Connector {
		
		public var m:Sprite;
		
		/**
		 * Set up the mask object.
		 */
		public function ConnectorMask() {
			m = new Sprite();
			addChild(m);
			mask = m;
		}
		
		/**
		 * Draw the mask.
		 */
		public override function render():void {
			m.graphics.clear();
			m.graphics.beginFill(0xffffff);
			m.graphics.drawRect(0, -5000, length, 10000);
		}
	}
}