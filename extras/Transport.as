package extras {
	
	import Box2DAS.*;
	import Box2DAS.Collision.*;
	import Box2DAS.Collision.Shapes.*;
	import Box2DAS.Common.*;
	import Box2DAS.Dynamics.*;
	import Box2DAS.Dynamics.Contacts.*;
	import Box2DAS.Dynamics.Joints.*;
	import cmodule.Box2D.*;
	import wck.*;
	import shapes.*;
	import misc.*;
	import extras.*;
	import flash.utils.*;
	import flash.events.*;
	import flash.display.*;
	import flash.text.*;
	import flash.geom.*;

	public class Transport extends BodyShape {
		
		[Inspectable(defaultValue='')]
		public var transportToWorldClass:String = '';
		
		[Inspectable(defaultValue='')]
		public var transportToName:String = '';
		
		public var contacts:ContactList;
		
		public override function create():void {
			isSensor = true;
			density = 0;
			if(transportToName != '') {
				reportBeginContact = true;
				reportEndContact = true;
				super.create();
				listenWhileVisible(world, World.TIME_STEP, processContacts, false, -5);
				contacts = new ContactList();
				contacts.listenTo(this);
			}
			else {
				super.create();
			}
		}
		
		public function processContacts(e:Event):void {
			var cl:Array = contacts.values;
			for(var i:int = 0; i < cl.length; ++i) {
				transport((cl[i].relatedObject as BodyShape).body);
			}
			contacts.clean();
		}
		
		public function transport(b:BodyShape):void {
			var bus:TransportBus = new TransportBus();
			bus.from = this;
			bus.bodyShape = b;
			if(transportToWorldClass != '') {
				/*if(b == world.focus) {
					var toWorldClass:Class = getDefinitionByName(transportToWorldClass) as Class;
					bus.toWorld = new toWorldClass() as World;
					bus.to = bus.toWorld.getChildByName(transportToName) as Transport;
				}*/
			}
			else {
				bus.to = world.getChildByName(transportToName) as Transport;
			}
			bus.fromPos = Util.localizePoint(this, bus.bodyShape);
			if(bus.to) {
				bus.toPos = Util.localizePoint(bus.toWorld ? bus.toWorld : world, bus.to, bus.fromPos);
				startTransport(bus);
			}
		}
		
		public function generate(b:BodyShape, p:Point = null):void {
			var bus:TransportBus = new TransportBus();
			bus.to = this;
			bus.bodyShape = b;
			bus.generated = true;
			if(p) {
				bus.toPos = p;
			}
			else {
				bus.toPos = Util.localizePoint(world, this);
			}
			startEject(bus);
		}
		
		public function startTransport(bus:TransportBus):void {
			endTransport(bus);
		}
		
		public function reEject(bus:TransportBus):void {
			bus.reEjected = true;
			startEject(bus);
		}
		
		public function endTransport(bus:TransportBus):void { 
			Util.remove(bus.bodyShape);
			if(bus.toWorld) {
				Util.replace(world, bus.toWorld);
				//bus.toWorld.focus = bus.bodyShape;
				bus.from = null; // So the other world doesn't stick around in memory.
			}
			if(bus.to) {
				bus.to.startEject(bus);
			}
		}
		
		public function startEject(bus:TransportBus):void {
			endEject(bus);
		}
		
		public function endEject(bus):void {
			bus.bodyShape.x = bus.toPos.x;
			bus.bodyShape.y = bus.toPos.y;
			if(contacts) {
				contacts.ignore(bus.bodyShape, 10);			
			}
			Util.addChildAbove(bus.bodyShape, this, false);
		}
	}
}