package misc {
	
	import flash.display.*;
	import flash.utils.*;
	import flash.geom.*;
	
	/**
	 * Connects two display objects via a line.
	 */
	public class ConnectorLine extends Connector {
				
		public var thickness:Number = 2;
		public var color:Number = 0x888888;
		
		/**
		 * Draw a line from (0, 0) to (length, 0).
		 */
		public override function render():void {
			graphics.clear();
			graphics.lineStyle(thickness, color);
			graphics.moveTo(0, 0);
			graphics.lineTo(length, 0);
		}
	}
}