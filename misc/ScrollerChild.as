package misc {
 
	import misc.*;
	import flash.display.*;
	import flash.events.*;
	import flash.utils.*;
	import flash.geom.*;
	import flash.ui.*;
 
	/**
	 * An entity that is a descendent of a "Scroller" entity.
	 */
	public class ScrollerChild extends Entity {
 
		/// Setting this to true will make the Scroller focus on this object. If you set this to true
		/// for more than one ScrollerChild in a Scroller, only one will become the focus.
		[Inspectable(defaultValue=false)]
		public var focusOn:Boolean = false;
 
		/**
		 * Tell the Scroller to focus on this object if focusOn == true.
		 */
		public override function create():void { 
			super.create();
			if(focusOn) {
				var s:Scroller = Util.findAncestorOfClass(this, Scroller) as Scroller;
				if(s) {
					s.focus = this;
				}
			}
		}
	}
}