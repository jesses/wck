package wck {
	
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
	import fl.motion.*;
	
	/**
	 * Wraps both b2Body and b2Shape and provides inspectable properties that can be edited in flash. Some properties of BodyShapes:
	 
	 * 1. If a BodyShape has another BodyShape as an
	 * parent/ancestor, it will act as a shape within that body. It it only has a World as an ancestor, it will act as a Body. BodyShapes can be moved
	 * around the display hierarchy and their role will shift depending on their ancestery. 
	 *
	 * 2. Many shapes can be defined per BodyShape to create complex shapes that are a single symbol in Flash. See defineShapes().
	 *
	 * 3. The transformation of the BodyShape relative to the world (position, rotation, scale & skew), will be applied to the 
	 * shape definitions before they are sent to Box2d - the visual representation of the shape will always match the physical simulation (with limits).
	 * A BodyShape can have many other DisplayObjects (with their own transforms) between it and the world.
	 *
	 */
	public class BodyShape extends ScrollerChild {
		
		/// The Joint-extending class to use for mouse dragging. This can be set to provide a visual, custom mouse joint.
		public static var dragJointClass:Class = Joint;
		
		/// Base strength of the drag mouse joint.
		public static var dragJointStrength:Number = 100;
		
		/// Added to the strength of the mouse joint - multiplied by the mass of the body being dragged.
		public static var dragJointMassFactor:Number = 200;
		
		/// See the Box2d documentation of b2Body for explanation of these variables:
		
		[Inspectable(defaultValue=0.0)]
		public var linearVelocityX:Number = 0.0;

		[Inspectable(defaultValue=0.0)]
		public var linearVelocityY:Number = 0.0;
		
		[Inspectable(defaultValue=0.0)]
		public var angularVelocity:Number = 0.0;
		
		[Inspectable(defaultValue=0.0)]
		public var linearDamping:Number = 0.0;
		
		[Inspectable(defaultValue=0.0)]
		public var angularDamping:Number = 0.0;
		
		[Inspectable(defaultValue=true)]
		public var autoSleep:Boolean = true;
		
		[Inspectable(defaultValue=true)]
		public var awake:Boolean = true;
		
		[Inspectable(defaultValue=false)]
		public var fixedRotation:Boolean = false;
		
		[Inspectable(defaultValue=false)]
		public var bullet:Boolean = false;
		
		[Inspectable(defaultValue="Dynamic",enumeration="Static,Kinematic,Dynamic")]
		public var type:String = 'Dynamic';
		
		[Inspectable(defaultValue=true)]
		public var active:Boolean = true;
		
		[Inspectable(defaultValue=1)]
		public var inertiaScale:Number = 1;
		
		/// See the Box2d documentation of b2Fixture for explanations of these variables:
	
		[Inspectable(defaultValue=0.2)]
		public var friction:Number = 0.2;
		
		[Inspectable(defaultValue=0.0)]
		public var restitution:Number = 0.0;
		
		[Inspectable(defaultValue=1.0)]
		public var density:Number = 1.0;
		
		[Inspectable(defaultValue='0x0001')]
		public var categoryBits:String = '0x0001';
	
		[Inspectable(defaultValue='0xFFFF')]
		public var maskBits:String = '0xFFFF';
	
		[Inspectable(defaultValue=0)]
		public var groupIndex:int = 0;
		
		[Inspectable(defaultValue=false)]
		public var isSensor:Boolean = false;
		
		/// This will force the BodyShape to use the ground as a body and act as a shape.
		[Inspectable(defaultValue=false)]
		public var isGround:Boolean = false;
		
		/// The following variables indicate if this body / shape needs contact events to be reported.
		
		/// Dispatch "onContactBegin" events?
		[Inspectable(defaultValue=false)]
		public var reportBeginContact:Boolean = false;
		
		/// Dispatch "onContactEnd" events?
		[Inspectable(defaultValue=false)]
		public var reportEndContact:Boolean = false;
		
		/// Dispatch "onPreSolve" events?
		[Inspectable(defaultValue=false)]
		public var reportPreSolve:Boolean = false;
		
		/// Dispatch "onPostSolve" events? 
		[Inspectable(defaultValue=false)]
		public var reportPostSolve:Boolean = false;
		
		/// Apply gravity?
		[Inspectable(defaultValue=true)]
		public var applyGravity:Boolean = true;
		
		/// Create a mouse joint to let the user move bodies around?
		[Inspectable(defaultValue=true)]
		public var allowDragging:Boolean = true;
		
		/// If the body is animated within flash via tweens or other means, setting this to true will create mouse joints
		/// to keep the b2body in sync with the display object, rather than the other way around.
		[Inspectable(defaultValue=false)]
		public var tweened:Boolean = false;

		public var world:World = null;
		public var selfBody:Boolean = false; /// Is this BodyShape acting as b2Body?
		public var body:BodyShape = null;
		public var b2body:b2Body = null;
		public var b2shapes:Array = null; /// Actually an array of b2Fixtures - Get the shape by fixture.m_shape.
		public var matrix:Matrix = null; /// A transform matrix that helps with Box2d -> BodyShape coordinate translation.
		public var gravity:V2; /// The last gravity vector applied to this body (only set when acting as a body).
		public var dragJoint:Joint;
		public var tweenJoint1:b2MouseJoint;
		public var tweenJoint2:b2MouseJoint;
		
		public var bufferedShapes:Array = [];
		
		public function shape(shapeFunc:Function, ...args):void {
			if(world) {
				shapeFunc.apply(this, args);
			}
			else {
				bufferedShapes.push([shapeFunc, args]);
			}
		}
		
		public function createBufferedShapes():void {
			for(var i:int = 0; i < bufferedShapes.length; ++i) {
				bufferedShapes[i][0].apply(this, bufferedShapes[i][1]);
			}
			bufferedShapes = [];
		}
		
		/**
		 * Override this to define the shapes that should be created. Use the helper methods below to
		 * make sure the shapes have the correct transform and properties.
		 *
		 * box();
		 * box(5, 10);
		 * circle(20);
		 * polygon([[0,0],[10,0],[0,10]]);
		 * etc.
		 */
		public function shapes():void {
		}
		
		/**
		 * Define a box. If you don't pass a halfWidth or halfHeight, it'll try to calculate them from the BodyShape's visible width & height.
		 * angle is in degrees. The BodyShape may be skewed, in which case the physical shape defined will not be a rectangle or a box.
		 */
		public function box(w:Number = 0, h:Number = 0, pos:V2 = null, angle:Number = 0):b2Fixture {
			pos ||= new V2();
			if(w == 0 || h == 0) {
				var b:Rectangle = Util.bounds(this);
				if(w == 0) w = b.width;
				if(h == 0) h = b.height;
			}
			var halfWidth = w / 2;
			var halfHeight = h / 2;
			var vertices:Vector.<V2> = Vector.<V2>([
				new V2(-halfWidth, -halfHeight), 
				new V2(halfWidth, -halfHeight), 
				new V2(halfWidth, halfHeight), 
				new V2(-halfWidth, halfHeight)
			]);
			orientVertices(vertices, pos, angle);
			return polygon(vertices);
		}
		
		/**
		 * Define a circle. If you don't pass a radius, it'll guess one from the width. Note: Skew transforms wont work, neither will non-uniform XY
		 * scaling (an oval shape).
		 */
		public function circle(radius:Number = 0, pos:V2 = null):b2Fixture {
			pos ||= new V2();
			if(radius == 0) {
				var b:Rectangle = Util.bounds(this);
				radius = b.width / 2;
			}
			var v1:V2 = pos.clone();
			var v:Vector.<V2> = Vector.<V2>([v1, new V2(v1.x + radius, v1.y)]);
			transformVertices(v);
			b2Def.circle.m_radius = v[1].x - v[0].x;
			b2Def.circle.m_p.x = v[0].x;
			b2Def.circle.m_p.y = v[0].y;
			initFixtureDef();
			b2Def.fixture.shape = b2Def.circle;
			return createFixture();
		}
		
		/**
		 * Define a polygon with a variable number of sides.
		 */
		public function polyN(sides:uint = 5, radius:Number = 0, pos:V2 = null, angle:Number = 0):b2Fixture {
			pos ||= new V2();
			if(radius == 0) {
				radius = Util.bounds(this).top;
			}
			var vertices:Vector.<V2> = new Vector.<V2>();
			for(var i:uint = 0; i < sides; ++i) {
				vertices.push(new V2(0, radius).rotate(i / sides * Math.PI * 2));
			}
			orientVertices(vertices, pos, angle);
			return polygon(vertices);
		}
		
		/**
		 *
		 */
		public function arc(degrees:Number = 360, sides:uint = 0, radius:Number = 0, pos:V2 = null, angle:Number = 0, detail:Number = 4):Array {
			pos ||= new V2();
			if(radius == 0) {
				radius = Util.bounds(this).top;
			}
			if(sides == 0) {
				/// Use the size of the arc to determine how many sides to use. The distance between <0,r> and <r,0> relative to the world
				/// is a good indicator of how big the arc is.
				var d = Point.distance(
					Util.localizePoint(world, this, new Point(radius, 0)),
					Util.localizePoint(world, this, new Point(0, radius))
				);
				sides = Math.round(d / world.scale * detail);
			}
			sides = Math.max(sides, 12); // Arbitrary minimum - but since <0,0> is part of every poly, forces them to be convex.
			var rad:Number = Util.D2R * degrees;
			var polys:Array = [];
			for(var i:uint = 0; i < sides; i += 4) {
				var v:Vector.<V2> = Vector.<V2>([new V2()]);
				var j2:uint = Math.min(i + 4, sides);
				for(var j:uint = i; j <= j2; ++j) {
					v.push(new V2(0, radius).rotate(j / sides * rad));
				}
				orientVertices(v, pos, angle);
				polys.push(polygon(v));
			}
			return polys;
		}
		
		/**
		 *
		 */
		public function lineArc(degrees:Number = 360, sides:uint = 0, radius:Number = 0, pos:V2 = null, angle:Number = 0, detail:Number = 8):Array {
			pos ||= new V2();
			if(radius == 0) {
				radius = Util.bounds(this).top;
			}
			if(sides == 0) {
				/// Use the size of the arc to determine how many sides to use. The distance between <0,r> and <r,0> relative to the world
				/// is a good indicator of how big the arc is.
				var d = Point.distance(
					Util.localizePoint(world, this, new Point(radius, 0)),
					Util.localizePoint(world, this, new Point(0, radius))
				);
				sides = Math.round(d / world.scale * detail);
			}
			var rad:Number = Util.D2R * degrees;
			var polys:Array = [];
			var v:Vector.<V2> = Vector.<V2>([new V2(0, radius)]);
			for(var i:uint = 0; i < sides; ++i) {
				v[1] = new V2(0, radius).rotate((i + 1) / sides * rad);
				orientVertices(v, pos, angle);
				polys.push(line(v[0], v[1]));
				v[0].x = v[1].x;
				v[0].y = v[1].y;
			}
			return polys;
		}
		
		/**
		 *
		 */
		public function line(v1:V2 = null, v2:V2 = null):b2Fixture {
			if(!(v1 && v2)) {
				var b:Rectangle = Util.bounds(this);
				v1 ||= new V2(0, b.top);
				v2 ||= new V2(0, b.bottom);
			}
			return polygon(Vector.<V2>([v1, v2]));
		}
		
		
		
		/**
		 * Define a polygon. Pass in an array of vertices in [[x, y], [x, y], ...] format
		 */
		public function poly(vertices:Array):b2Fixture {
			var v:Vector.<V2> = new Vector.<V2>();
			for(var i:int = 0; i < vertices.length; ++i) {
				v.push(new V2(vertices[i][0], vertices[i][1]));
			}
			return polygon(v);
		}
		
		/**
		 * Takes an array of polygon points - each is sent to the "poly" function.
		 */
		public function polys(vertices:Array):Array {
			var a:Array = [];
			for(var i:int = 0; i < vertices.length; ++i) {
				a.push(poly(vertices[i]));
			}
			return a;
		}
		
		/**
		 * Like poly(), but takes a V2 vector.
		 */
		public function polygon(vertices:Vector.<V2>):b2Fixture {
			transformVertices(vertices);
			/// If the bodyshape has been flipped on its y or x axis, then vertices are in the wrong direction.
			var sx:Number = MatrixTransformer.getScaleX(matrix);
			var sy:Number = MatrixTransformer.getScaleY(matrix);
			var kx:Number = MatrixTransformer.getSkewX(matrix);
			var ky:Number = MatrixTransformer.getSkewY(matrix);
			if(sx < 0 || sy < 0 || Math.abs(kx) > 90 || Math.abs(ky) > 90) {
				flipVertices(vertices);
			}
			b2Def.polygon.Set(vertices);
			initFixtureDef();
			b2Def.fixture.shape = b2Def.polygon;
			return createFixture();
		}
		
		/**
		 *
		 */
		public function flipVertices(v:Vector.<V2>):void {
			var low:uint = 0;
			var high:uint = v.length - 1;
			var tmp:Number;
			while(high > low) {
				tmp = v[low].x;
				v[low].x = v[high].x;
				v[high].x = tmp;
				tmp = v[low].y;
				v[low].y = v[high].y;
				v[high].y = tmp;
				++low;
				--high;
			}
		}
		
		/**
		 * Initialize fixture def properties.
		 */
		public function initFixtureDef():void {
			if(b2Def.fixture.userData == this) {
				return; /// Fixture def is already in sync.
			}
			b2Def.fixture.friction = friction;
			b2Def.fixture.restitution = restitution;
			b2Def.fixture.density = density;
			b2Def.fixture.filter.categoryBits = parseInt(categoryBits);
			b2Def.fixture.filter.maskBits = parseInt(maskBits);
			b2Def.fixture.filter.groupIndex = groupIndex;
			b2Def.fixture.isSensor = isSensor;
			b2Def.fixture.userData = this;
		}
		
		/**
		 * Add the shape to the b2shapes array and sets common shape properties.
		 */
		public function createFixture():b2Fixture {
			var fix:b2Fixture = new b2Fixture(b2body, b2Def.fixture);
			b2shapes.push(fix);
			fix.m_reportBeginContact = fix.m_userData.reportBeginContact;
			fix.m_reportEndContact = fix.m_userData.reportEndContact;
			fix.m_reportPreSolve = fix.m_userData.reportPreSolve;
			fix.m_reportPostSolve = fix.m_userData.reportPostSolve;
			return fix;
		}
		
		/**
		 * Takes a polygon (list of vertices) and rotates, repositions them.
		 */
		public function orientVertices(vertices:Vector.<V2>, pos:V2 = null, angle:Number = 0):void {
			pos ||= new V2();
			if(angle != 0 || pos.x != 0 || pos.y != 0) {
				for(var i:uint = 0; i < vertices.length; ++i) {
					vertices[i].rotate(Util.D2R * angle).add(pos);
				}
			}
		}
		
		/**
		 * Takes an array of vertices ([[x,y],[x,y]]) and transforms them based on their MovieClip transformation.
		 */
		public function transformVertices(vertices:Vector.<V2>):void {
			for(var i:int = 0; i < vertices.length; ++i) {
				vertices[i] = transformVertex(vertices[i]);
			}
		}
		
		/**
		 * Will take a flash X,Y within this object and return a new X,Y reflecting where that
		 * X, Y is in Box2d dimentions.
		 */
		public function transformVertex(xy:V2):V2 {
			var p:Point = matrix.transformPoint(xy.toP());
			return new V2(p.x / world.scale, p.y / world.scale);
		}
		
		/**
		 * Determine the role this BodyShape should play and either create a body (possibly with shapes) or shapes (in another body).
		 */
		public override function create():void {
			world = Util.findAncestorOfClass(this, World) as World;
			if(!world || world.disabled) {
				return;
			}
			world.ensureCreated();
			if(isGround) {
				world.doOutsideTimeStep(createShapes);
			}
			else {
				body = Util.findAncestorOfClass(this, BodyShape) as BodyShape;
				if(body) {
					if(!body.disabled) {
						body.ensureCreated();
						body = body.body;
						world.doOutsideTimeStep(createShapes);
					}
				}
				else if(world) {
					world.doOutsideTimeStep(createBody);
				}
			}
			super.create();
		}
		
		/**
		 * Destroy either as a body or a shape. Doesn't do the destroying if the body or world ancestor was destroyed,
		 * because that implicitly destroys the body / shapes.
		 */
		public override function destroy():void {
			if(world && world.created && !world.disabled) {
				if(selfBody) {
					world.doOutsideTimeStep(destroyBody);
				}
				else if(body && !body.disabled && (body.created || body.isGround)) {
					world.doOutsideTimeStep(destroyShapes);
				}
			}
		}
		
		/**
		 * BodyShape is assuming the role of a b2Body. Create the body and record the transformation matrix so we know
		 * how to translate box2d transforms into Flash transforms. Also allow the body to define its own shapes within
		 * its self.
		 */
		public function createBody():void {
			selfBody = true;
			body = this;
			matrix = Util.localizeMatrix(world, this);
			b2Def.body.position.x = matrix.tx / world.scale;
			b2Def.body.position.y = matrix.ty / world.scale;
			b2Def.body.angle = MatrixTransformer.getRotationRadians(matrix);
			b2Def.body.linearVelocity.x = linearVelocityX;
			b2Def.body.linearVelocity.y = linearVelocityY;
			b2Def.body.angularVelocity = angularVelocity;
			b2Def.body.linearDamping = linearDamping;
			b2Def.body.angularDamping = angularDamping;
			b2Def.body.allowSleep = autoSleep;
			b2Def.body.awake = awake;
			b2Def.body.fixedRotation = fixedRotation;
			b2Def.body.bullet = bullet;
			b2Def.body.type = {Static: b2Body.b2_staticBody, Kinematic: b2Body.b2_kinematicBody, Dynamic: b2Body.b2_dynamicBody}[type];
			b2Def.body.active = active;
			b2Def.body.inertiaScale = inertiaScale;
			b2Def.body.userData = this;
			b2body = new b2Body(world.b2world, b2Def.body);
			MatrixTransformer.setRotation(matrix, 0);
			matrix.tx = 0;
			matrix.ty = 0;
			b2shapes = [];
			createBufferedShapes();
			shapes();
			if(!b2body.IsStatic()) {
				if(tweened) {
					listenWhileVisible(world, World.TIME_STEP, updateTween, false, 1);
					lastP = Util.localizePoint(world, this);
					lastR = Util.localizeRotation(world, this);
				}
				else if(parent == world) {
					listenWhileVisible(world, World.TIME_STEP, updateBodyMatrixSimple, false, -10);
				}
				else {
					listenWhileVisible(world, World.TIME_STEP, updateBodyMatrix, false, -10);
				}
			}
		}
		
		/**
		 * Translation the new tweened position & rotation into velocities for Box2d.
		 */
		public var lastP:Point;
		public var lastR:Number;
		public function updateTween(e:Event):void {
			var p2:Point = Util.localizePoint(world, this);
			var r2:Number = Util.findBetterAngleTarget(lastR, Util.localizeRotation(world, this));
			/// I'm really not sure why 0.05 / 20 are the magic numbers here, but they work perfect.
			b2body.SetLinearVelocity(new V2(p2.x - lastP.x, p2.y - lastP.y).divideN(world.scale * 0.05));
			b2body.SetAngularVelocity((r2 - lastR) * Util.D2R * 20);
			lastP = p2;
			lastR = r2;
		}
		
		/**
		 * BodyShape is acting as a shape on another body. Create shapes on that body.
		 */
		public function createShapes():void {
			if(isGround) {
				matrix = Util.localizeMatrix(world, this);
				body = this;
				b2body = world.b2world.m_groundBody;
			}
			else {
				body.ensureCreated();
				b2body = body.b2body;
				matrix = Util.localizeMatrix(body, this);
				matrix.concat(body.matrix);
			}
			selfBody = false;
			b2shapes = [];
			shapes();
			createBufferedShapes();
		}
		
		/**
		 * Destroy a BodyShape acting as a Body. Any shapes it creates for itself are automatically destroyed in Box2d.
		 */
		public function destroyBody():void {
			b2body.destroy();
		}
		
		/**
		 * Destroy a BodyShape acting as a shape collection on another BodyShape.
		 */
		public function destroyShapes():void { 
			for(var i:int = 0; i < b2shapes.length; i++) {
				b2shapes[i].destroy();
			}
		}
		
		/**
		 * Read the current position and rotation from Box2d and reposition the visible BodyShape.
		 */
		public function updateBodyMatrix(e:Event):void {
			var m:Matrix = matrix.clone();
			m.rotate(b2body.m_sweep.a % Util.PI2);
			m.translate(b2body.m_xf.position.x * world.scale, b2body.m_xf.position.y * world.scale);
			if(parent != world) {
				/// This might introduce some overhead, but is necessary if the body is not a direct descendant of the world. There
				/// may be transformations between the world and the body we are not aware of and they might change. It wouldn't be necessary if the objects
				/// between the world and the body had a transform that never changed... can't be sure of that though. Maybe have a flag
				/// the user can set to tell the engine these transforms never change and to skip this step?
				var fix:Matrix = Util.localizeMatrix(world, parent);
				fix.invert();
				m.concat(fix);
			}
			transform.matrix = m;
		}
		
		/**
		 * A simpler version of "updateBodyMatrix" for when the body is a direct descendent of the world. May have performance gains.
		 */
		public function updateBodyMatrixSimple(e:Event):void {
			rotation = b2body.m_sweep.a * Util.R2D % 360;
			x = b2body.m_xf.position.x * world.scale;
			y = b2body.m_xf.position.y * world.scale;
		}
				
		/**
		 * Start dragging via a mouse joint.
		 */
		public override function handleDragStart(e:Event):void {
			if(b2body.IsDynamic() && world.allowDragging && allowDragging) {
				b2body.SetAwake(true);
				super.handleDragStart(e);
				createDragJoint();
				dragJoint.type = "Mouse";
				var p:Point = Input.mousePositionIn(world);
				dragJoint.x = p.x;
				dragJoint.y = p.y;
				dragJoint.bodyShape1 = this;
				world.addChild(dragJoint);
			}
		}
		
		/**
		 * This could be overridden to further customize the mouse joint.
		 */
		public function createDragJoint() {
			dragJoint = new dragJointClass() as Joint;
			dragJoint.motorStrength = dragJointStrength + (b2body.m_mass * dragJointMassFactor);
			dragJoint.frequencyHz = 999999;
			dragJoint.dampingRatio = 0;
			dragJoint.collideConnected = true;
		}
		
		/**
		 * Move the target of the mouse joint.
		 */
		public override function handleDragStep(e:Event):void {
			var p:Point = Input.mousePositionIn(world);
			(dragJoint.b2joint as b2MouseJoint).SetTarget(new V2(p.x / world.scale, p.y / world.scale));
		}
		
		/**
		 * Destroy the mouse joint.
		 */
		public override function handleDragStop(e:Event):void {
			super.handleDragStop(e);
			Util.remove(dragJoint);
		}
	}
}