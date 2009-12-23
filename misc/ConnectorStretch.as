package misc {
	
	import flash.display.*;
	import flash.utils.*;
	import flash.geom.*;
	
	/**
	 * A connector that "stretches" a predrawn "line" between two objects.
	 */
	public class ConnectorStretch extends Connector {
		
		/**
		 * Set the width of the connector to the distance between the objects.
		 */
		public override function render():void {
			var r:Number = rotation;
			rotation = 0;
			width = length;
			rotation = r;
		}
	}
}