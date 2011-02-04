/*

To compile the SWC:

alc-on; g++ Box2D.c -I.. -O3 -Wall -swc -DOSX -o Box2D.swc; alc-off

To see the temporary AS files:

export ACHACKS_TMPS=1


*/
#include "Box2DAS/Box2D.h";

char* s;
int o;

#define F(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():Number { return mem._mrf(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:Number):void { mem._mwf(_ptr + %d, v); }", #member, o); TS(s);

#define B(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():Boolean { return mem._mru8(_ptr + %d) == 1; }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:Boolean):void { mem._mw8(_ptr + %d, v ? 1 : 0); }", #member, o); TS(s);

#define I(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():int { return mem._mr32(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:int):void { mem._mw32(_ptr + %d, v); }", #member, o); TS(s);
		
#define U16(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():int { return mem._mru16(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:int):void { mem._mw16(_ptr + %d, v); }", #member, o); TS(s);
	
#define S16(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():int { return mem._mrs16(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:int):void { mem._mw16(_ptr + %d, v); }", #member, o); TS(s);

#define U8(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():int { return mem._mru8(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:int):void { mem._mw8(_ptr + %d, v); }", #member, o); TS(s);
	
#define S8(class, member) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():int { return mem._mrs8(_ptr + %d); }", #member, o); TS(s); \
	sprintf(s, "public function set %s(v:int):void { mem._mw8(_ptr + %d, v); }", #member, o); TS(s);
	
#define V(class, member, counter) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public function get %s():Vector.<V2> { return readVertices(_ptr + %d, %s); }", #member, o, #counter); TS(s); \
	sprintf(s, "public function set %s(v:Vector.<V2>):void { writeVertices(_ptr + %d, v); %s = v.length; }", #member, o, #counter); TS(s);

#define R(class, member, rclass) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public var _%s:%s;", #member, #rclass); TS(s); \
	sprintf(s, "public function get %s():%s { return _%s; }", #member, #rclass, #member); TS(s); \
	sprintf(s, "public function set %s(v:%s):void { mem._mw32(_ptr + %d, v._ptr); _%s = v; }", #member, #rclass, o, #member); TS(s);

#define C(class, member, rclass) \
	o = (int)offsetof(class, member); \
	sprintf(s, "public var %s:%s; // %s = new %s(_ptr + %d);", #member, #rclass, #member, #rclass, o); TS(s);

#define G(global) \
	sprintf(s, "public static var %s = %d;", #global, &global); TS(s);

AS3_Val emptyFunction(void* data, AS3_Val args) {
	/// empty.
	return AS3_Null();
}

AS3_Val testFunction(void* data, AS3_Val args) {
	AS3_Val test;
	AS3_ArrayValue(args, "AS3ValType", &test);
	return AS3_Ptr(test);
}

int main() {



	
	T(----------);

	T(b2Vec2);
	F(b2Vec2, x);
	F(b2Vec2, y);
	
	T(----------);

	T(b2Vec3);
	F(b2Vec3, x);
	F(b2Vec3, y);
	F(b2Vec3, z);

	T(----------);
	
	T(b2Mat22);
	C(b2Mat22, col1, b2Vec2);
	C(b2Mat22, col2, b2Vec2);

	T(----------);
	
	T(b2Mat33);
	C(b2Mat33, col1, b2Vec3);
	C(b2Mat33, col2, b2Vec3);
	C(b2Mat33, col3, b2Vec3);

	T(----------);
	
	T(b2Transform);
	C(b2Transform, position, b2Vec2);
	C(b2Transform, R, b2Mat22);

	T(----------);
	
	T(b2Sweep);
	C(b2Sweep, localCenter, b2Vec2);
	C(b2Sweep, c0, b2Vec2);
	C(b2Sweep, c, b2Vec2);
	F(b2Sweep, a0);
	F(b2Sweep, a);
	//F(b2Sweep, t0); // !!!
	F(b2Sweep, alpha0);

	T(----------);
	
	T(b2MassData);
	F(b2MassData, mass);
	C(b2MassData, center, b2Vec2);
	F(b2MassData, I);
	
	T(----------);
	
	T(b2Filter);
	U16(b2Filter, categoryBits);
	U16(b2Filter, maskBits);
	S16(b2Filter, groupIndex);
	
	T(----------);
	
	T(b2AABB);
	C(b2AABB, lowerBound, b2Vec2);
	C(b2AABB, upperBound, b2Vec2)
	
	T(----------);
	
	T(b2World);
	I(b2World, m_flags);
	I(b2World, m_bodyCount);
	I(b2World, m_jointCount);
	C(b2World, m_gravity, b2Vec2);
	B(b2World, m_allowSleep);
	// C(b2World, m_groundBody, b2Body); // !!!!!!
	C(b2World, m_contactManager, b2ContactManager);
	B(b2World, m_warmStarting);
	B(b2World, m_continuousPhysics);
	
	T(----------);
	
	T(b2ContactID);
	I(b2ContactID, key);
	//U8(b2ContactID, features.referenceEdge); // !!!
	//U8(b2ContactID, features.incidentEdge); // !!!
	//U8(b2ContactID, features.incidentVertex); // !!!
	//U8(b2ContactID, features.flip); // !!!
	U8(b2ContactID, cf.indexA);
	U8(b2ContactID, cf.indexB);
	U8(b2ContactID, cf.typeA);
	U8(b2ContactID, cf.typeB);
	
	T(----------);
	
	T(b2ManifoldPoint);
	C(b2ManifoldPoint, localPoint, b2Vec2); // !!!
	F(b2ManifoldPoint, normalImpulse); // !!!
	F(b2ManifoldPoint, tangentImpulse); // !!!
	C(b2ManifoldPoint, id, b2ContactID); // !!!
	B(b2ManifoldPoint, isNew); // !!!
	
	T(----------);
	
	T(b2Manifold);
	//C(b2Manifold, localPlaneNormal, b2Vec2); // !!!
	C(b2Manifold, localNormal, b2Vec2);	
	C(b2Manifold, localPoint, b2Vec2); // !!!
	S8(b2Manifold, type); // !!!
	I(b2Manifold, pointCount); // !!!
	C(b2Manifold, points[0], b2ManifoldPoint); // !!!
	C(b2Manifold, points[1], b2ManifoldPoint); // !!!
	
	T(----------);
	
	T(b2WorldManifold);
	C(b2WorldManifold, normal, b2Vec2); // !!!
	C(b2WorldManifold, points[0], b2Vec2); // !!!
	C(b2WorldManifold, points[1], b2Vec2); // !!!
	
	T(----------);
	
	T(b2Contact);
	I(b2Contact, m_flags);
	R(b2Contact, m_fixtureA, b2Fixture);
	R(b2Contact, m_fixtureB, b2Fixture);
	C(b2Contact, m_manifold, b2Manifold);
	I(b2Contact, m_indexA);
	I(b2Contact, m_indexB);
	I(b2Contact, m_toiCount);
	F(b2Contact, m_toi);
	B(b2Contact, frictionDisabled);
	I(b2Contact, m_next);
	I(b2Contact, m_prev);
	
	T(----------);

	T(b2BodyDef);
	I(b2BodyDef, userData);
	C(b2BodyDef, position, b2Vec2);
	F(b2BodyDef, angle);
	C(b2BodyDef, linearVelocity, b2Vec2);
	F(b2BodyDef, angularVelocity);
	F(b2BodyDef, linearDamping);
	F(b2BodyDef, angularDamping);
	B(b2BodyDef, allowSleep);
	B(b2BodyDef, awake);
	B(b2BodyDef, fixedRotation);
	B(b2BodyDef, bullet);
	S16(b2BodyDef, type);
	B(b2BodyDef, active);
	F(b2BodyDef, inertiaScale);
	
	T(----------);
	
	T(b2Body);
	U16(b2Body, m_flags);
	S16(b2Body, m_type);
	I(b2Body, m_islandIndex);
	C(b2Body, m_xf, b2Transform);
	C(b2Body, m_sweep, b2Sweep);
	C(b2Body, m_linearVelocity, b2Vec2);
	F(b2Body, m_angularVelocity);
	C(b2Body, m_force, b2Vec2);
	F(b2Body, m_torque);
	I(b2Body, m_fixtureCount);
	F(b2Body, m_mass);
	F(b2Body, m_invMass);
	F(b2Body, m_I);
	F(b2Body, m_invI);
	F(b2Body, m_linearDamping);
	F(b2Body, m_angularDamping);
	F(b2Body, m_sleepTime);
	I(b2Body, m_userData);
	C(b2Body, m_contactList, b2ContactEdge);
	C(b2Body, m_jointList, b2JointEdge);
	// F(b2Body, m_inertiaScale); // !!!
	
	T(----------);
	
	T(b2ContactEdge);
	C(b2ContactEdge, other, b2Body);
	C(b2ContactEdge, contact, b2Contact);
	C(b2ContactEdge, prev, b2ContactEdge);
	C(b2ContactEdge, next, b2ContactEdge);
	
	T(----------);
	
	T(b2JointEdge);
	C(b2JointEdge, other, b2Body);
	C(b2JointEdge, joint, b2Joint);
	C(b2JointEdge, prev, b2JointEdge);
	C(b2JointEdge, next, b2JointEdge);
	
	T(----------);
	
	T(b2Shape);
	S8(b2Shape, m_type);
	F(b2Shape, m_radius);
	F(b2Shape, m_area);
	
	T(----------);
	
	T(b2CircleShape);
	C(b2CircleShape, m_p, b2Vec2);
	
	T(----------);
	
	T(b2PolygonShape);
	C(b2PolygonShape, m_centroid, b2Vec2);
	V(b2PolygonShape, m_vertices, m_vertexCount);
	V(b2PolygonShape, m_normals, m_vertexCount);
	I(b2PolygonShape, m_vertexCount);
	
	T(----------);
	
	T(b2FixtureDef);
	R(b2FixtureDef, shape, b2Shape);
	I(b2FixtureDef, userData);
	F(b2FixtureDef, friction);
	F(b2FixtureDef, restitution);
	F(b2FixtureDef, density); 
	B(b2FixtureDef, isSensor);
	C(b2FixtureDef, filter, b2Filter);
	
	T(----------);
	
	T(b2Fixture);
	B(b2Fixture, m_reportBeginContact);
	B(b2Fixture, m_reportEndContact);
	B(b2Fixture, m_reportPreSolve);
	B(b2Fixture, m_reportPostSolve);
	// C(b2Fixture, m_aabb, b2AABB); // !!!
	R(b2Fixture, m_body, b2Body);
	R(b2Fixture, m_shape, b2Shape);
	F(b2Fixture, m_friction);
	F(b2Fixture, m_restitution);
	// I(b2Fixture, m_proxyId); // !!!
	I(b2Fixture, m_proxies);
	C(b2Fixture, m_filter, b2Filter);
	B(b2Fixture, m_isSensor);
	I(b2Fixture, m_userData);
	F(b2Fixture, m_density);
	F(b2Fixture, m_conveyorBeltSpeed);
	
	T(----------);
	
	T(b2FixtureProxy);
	C(b2FixtureProxy, aabb, b2AABB);
	I(b2FixtureProxy, proxyId);
	
	T(----------);
	
	T(b2JointDef);
	S16(b2JointDef, type);
	I(b2JointDef, userData);
	R(b2JointDef, bodyA, b2Body);
	R(b2JointDef, bodyB, b2Body);
	B(b2JointDef, collideConnected);
	
	T(----------);
	
	T(b2Joint);
	S16(b2Joint, m_type);
	R(b2Joint, m_bodyA, b2Body);
	R(b2Joint, m_bodyB, b2Body);
	B(b2Joint, m_islandFlag);
	B(b2Joint, m_collideConnected);
	F(b2Joint, m_userData);
	
	T(----------);
	
	T(b2DistanceJointDef);
	C(b2DistanceJointDef, localAnchorA, b2Vec2);
	C(b2DistanceJointDef, localAnchorB, b2Vec2);
	F(b2DistanceJointDef, length);
	F(b2DistanceJointDef, frequencyHz);
	F(b2DistanceJointDef, dampingRatio);
	
	T(----------);
	
	T(b2DistanceJoint);
	C(b2DistanceJoint, m_localAnchor1, b2Vec2);
	C(b2DistanceJoint, m_localAnchor2, b2Vec2);
	C(b2DistanceJoint, m_u, b2Vec2);
	F(b2DistanceJoint, m_frequencyHz);
	F(b2DistanceJoint, m_dampingRatio);
	F(b2DistanceJoint, m_gamma);
	F(b2DistanceJoint, m_bias);
	F(b2DistanceJoint, m_impulse);
	F(b2DistanceJoint, m_mass);
	F(b2DistanceJoint, m_length);
	
	T(----------);
	
	T(b2GearJointDef);
	R(b2GearJointDef, joint1, b2Joint);
	R(b2GearJointDef, joint2, b2Joint);
	F(b2GearJointDef, ratio);
	
	T(----------);
	
	T(b2GearJoint);
	R(b2GearJoint, m_ground1, b2Body);
	R(b2GearJoint, m_ground2, b2Body);
	R(b2GearJoint, m_revolute1, b2RevoluteJoint);
	R(b2GearJoint, m_prismatic1, b2PrismaticJoint);
	R(b2GearJoint, m_revolute2, b2RevoluteJoint);
	R(b2GearJoint, m_prismatic2, b2PrismaticJoint);
	C(b2GearJoint, m_groundAnchor1, b2Vec2);
	C(b2GearJoint, m_groundAnchor2, b2Vec2);
	C(b2GearJoint, m_localAnchor1, b2Vec2);
	C(b2GearJoint, m_localAnchor2, b2Vec2);
	F(b2GearJoint, m_constant);
	F(b2GearJoint, m_ratio);
	F(b2GearJoint, m_mass);
	F(b2GearJoint, m_impulse);
	C(b2GearJoint, m_J, b2Jacobian);
	
	T(----------);
	
	T(b2LineJointDef);
	C(b2LineJointDef, localAnchorA, b2Vec2);
	C(b2LineJointDef, localAnchorB, b2Vec2);
	C(b2LineJointDef, localAxisA, b2Vec2);
	B(b2LineJointDef, enableMotor);
	F(b2LineJointDef, maxMotorTorque);
	F(b2LineJointDef, motorSpeed);
	F(b2LineJointDef, frequencyHz);
	F(b2LineJointDef, dampingRatio);
	
	T(----------);
	
	T(b2LineJoint);
	C(b2LineJoint, m_localAnchorA, b2Vec2);
	C(b2LineJoint, m_localAnchorB, b2Vec2);
	C(b2LineJoint, m_localXAxisA, b2Vec2);
	C(b2LineJoint, m_localYAxisA, b2Vec2);
	
	/* Line joint has changed. 
	C(b2LineJoint, m_impulse, b2Vec2);
	F(b2LineJoint, m_motorMass);
	F(b2LineJoint, m_motorImpulse);
	F(b2LineJoint, m_lowerTranslation);
	F(b2LineJoint, m_upperTranslation);
	F(b2LineJoint, m_maxMotorForce);
	F(b2LineJoint, m_motorSpeed);
	B(b2LineJoint, m_enableLimit);
	B(b2LineJoint, m_enableMotor);
	S16(b2LineJoint, m_limitState);
	C(b2LineJoint, m_axis, b2Vec2);
	C(b2LineJoint, m_perp, b2Vec2);
	F(b2LineJoint, m_s1);
	F(b2LineJoint, m_s2);
	F(b2LineJoint, m_a1);
	F(b2LineJoint, m_a2); */

	C(b2LineJoint, m_ax, b2Vec2);
	C(b2LineJoint, m_ay, b2Vec2);
	F(b2LineJoint, m_sAx)
	F(b2LineJoint, m_sBx);
	F(b2LineJoint, m_sAy);
	F(b2LineJoint, m_sBy);

	F(b2LineJoint, m_mass);
	F(b2LineJoint, m_impulse);
	F(b2LineJoint, m_motorMass);
	F(b2LineJoint, m_motorImpulse);
	F(b2LineJoint, m_springMass);
	F(b2LineJoint, m_springImpulse);

	F(b2LineJoint, m_maxMotorTorque);
	F(b2LineJoint, m_motorSpeed);
	F(b2LineJoint, m_frequencyHz);
	F(b2LineJoint, m_dampingRatio);
	F(b2LineJoint, m_bias);
	F(b2LineJoint, m_gamma);

	B(b2LineJoint, m_enableMotor);
	
	T(----------);
	
	T(b2MouseJointDef);
	C(b2MouseJointDef, target, b2Vec2);
	F(b2MouseJointDef, maxForce);
	F(b2MouseJointDef, frequencyHz);
	F(b2MouseJointDef, dampingRatio);
	
	T(----------);
	
	T(b2MouseJoint);
	C(b2MouseJoint, m_localAnchor, b2Vec2);
	C(b2MouseJoint, m_target, b2Vec2);
	C(b2MouseJoint, m_impulse, b2Vec2);
	C(b2MouseJoint, m_C, b2Vec2);
	F(b2MouseJoint, m_maxForce);
	F(b2MouseJoint, m_frequencyHz);
	F(b2MouseJoint, m_dampingRatio);
	F(b2MouseJoint, m_beta);
	F(b2MouseJoint, m_gamma);
	
	T(----------);
	
	T(b2PrismaticJointDef);
	C(b2PrismaticJointDef, localAnchorA, b2Vec2);
	C(b2PrismaticJointDef, localAnchorB, b2Vec2);
	C(b2PrismaticJointDef, localAxis1, b2Vec2);
	F(b2PrismaticJointDef, referenceAngle);
	B(b2PrismaticJointDef, enableLimit);
	F(b2PrismaticJointDef, lowerTranslation);
	F(b2PrismaticJointDef, upperTranslation);
	B(b2PrismaticJointDef, enableMotor);
	F(b2PrismaticJointDef, maxMotorForce);
	F(b2PrismaticJointDef, motorSpeed);
	
	T(----------);
	
	T(b2PrismaticJoint);
	C(b2PrismaticJoint, m_localAnchor1, b2Vec2);
	C(b2PrismaticJoint, m_localAnchor2, b2Vec2);
	C(b2PrismaticJoint, m_localXAxis1, b2Vec2);
	C(b2PrismaticJoint, m_localYAxis1, b2Vec2);
	F(b2PrismaticJoint, m_refAngle);
	C(b2PrismaticJoint, m_axis, b2Vec2);
	C(b2PrismaticJoint, m_perp, b2Vec2);
	F(b2PrismaticJoint, m_s1);
	F(b2PrismaticJoint, m_s2);
	F(b2PrismaticJoint, m_a1);
	F(b2PrismaticJoint, m_a2);
	C(b2PrismaticJoint, m_impulse, b2Vec3);
	F(b2PrismaticJoint, m_motorMass);
	F(b2PrismaticJoint, m_motorImpulse);
	F(b2PrismaticJoint, m_lowerTranslation);
	F(b2PrismaticJoint, m_upperTranslation);
	F(b2PrismaticJoint, m_maxMotorForce);
	F(b2PrismaticJoint, m_motorSpeed);
	B(b2PrismaticJoint, m_enableLimit);
	B(b2PrismaticJoint, m_enableMotor);
	S16(b2PrismaticJoint, m_limitState);
	
	T(----------);
	
	T(b2PulleyJointDef);
	C(b2PulleyJointDef, groundAnchorA, b2Vec2);
	C(b2PulleyJointDef, groundAnchorB, b2Vec2);
	C(b2PulleyJointDef, localAnchorA, b2Vec2);
	C(b2PulleyJointDef, localAnchorB, b2Vec2);
	F(b2PulleyJointDef, lengthA);
	F(b2PulleyJointDef, maxLengthA);
	F(b2PulleyJointDef, lengthB);
	F(b2PulleyJointDef, maxLengthB);
	F(b2PulleyJointDef, ratio);
	
	T(----------);
	
	T(b2PulleyJoint);
	C(b2PulleyJoint, m_groundAnchor1, b2Vec2);
	C(b2PulleyJoint, m_groundAnchor2, b2Vec2);
	C(b2PulleyJoint, m_localAnchor1, b2Vec2);
	C(b2PulleyJoint, m_localAnchor2, b2Vec2);
	C(b2PulleyJoint, m_u1, b2Vec2);
	C(b2PulleyJoint, m_u2, b2Vec2);
	F(b2PulleyJoint, m_constant);
	F(b2PulleyJoint, m_ratio);
	F(b2PulleyJoint, m_maxLength1);
	F(b2PulleyJoint, m_maxLength2);
	F(b2PulleyJoint, m_pulleyMass);
	F(b2PulleyJoint, m_limitMass1);
	F(b2PulleyJoint, m_limitMass2);
	F(b2PulleyJoint, m_impulse);
	F(b2PulleyJoint, m_limitImpulse1);
	F(b2PulleyJoint, m_limitImpulse2);
	S16(b2PulleyJoint, m_state);
	S16(b2PulleyJoint, m_limitState1);
	S16(b2PulleyJoint, m_limitState2);
	
	T(----------);
	
	T(b2RevoluteJointDef);
	C(b2RevoluteJointDef, localAnchorA, b2Vec2);
	C(b2RevoluteJointDef, localAnchorB, b2Vec2);
	F(b2RevoluteJointDef, referenceAngle);
	B(b2RevoluteJointDef, enableLimit);
	F(b2RevoluteJointDef, lowerAngle);
	F(b2RevoluteJointDef, upperAngle);
	B(b2RevoluteJointDef, enableMotor);
	F(b2RevoluteJointDef, motorSpeed);
	F(b2RevoluteJointDef, maxMotorTorque);
	
	T(----------);
	
	T(b2RevoluteJoint);
	C(b2RevoluteJoint, m_localAnchor1, b2Vec2);
	C(b2RevoluteJoint, m_localAnchor2, b2Vec2);
	C(b2RevoluteJoint, m_impulse, b2Vec3);
	F(b2RevoluteJoint, m_motorImpulse);
	F(b2RevoluteJoint, m_motorMass);
	B(b2RevoluteJoint, m_enableMotor);
	F(b2RevoluteJoint, m_maxMotorTorque);
	F(b2RevoluteJoint, m_motorSpeed);
	B(b2RevoluteJoint, m_enableLimit);
	F(b2RevoluteJoint, m_referenceAngle);
	F(b2RevoluteJoint, m_lowerAngle);
	F(b2RevoluteJoint, m_upperAngle);
	S16(b2RevoluteJoint, m_limitState);
	
	T(----------);
	
	T(b2WeldJointDef);
	C(b2WeldJointDef, localAnchorA, b2Vec2);
	C(b2WeldJointDef, localAnchorB, b2Vec2);
	F(b2WeldJointDef, referenceAngle);
	
	T(----------);
	
	T(b2WeldJoint);
	C(b2WeldJoint, m_localAnchorA, b2Vec2);
	C(b2WeldJoint, m_localAnchorB, b2Vec2);
	F(b2WeldJoint, m_referenceAngle);
	C(b2WeldJoint, m_impulse, b2Vec3);
	C(b2WeldJoint, m_mass, b2Mat33);
	
	T(----------);
	
	T(b2RopeJointDef);
	C(b2RopeJointDef, localAnchorA, b2Vec2);
	C(b2RopeJointDef, localAnchorB, b2Vec2);
	F(b2RopeJointDef, maxLength);
	
	T(----------);
	
	T(b2RopeJoint);
	C(b2RopeJoint, m_localAnchorA, b2Vec2);
	C(b2RopeJoint, m_localAnchorB, b2Vec2);
	F(b2RopeJoint, m_maxLength);
	F(b2RopeJoint, m_length);
	C(b2RopeJoint, m_u, b2Vec2);
	C(b2RopeJoint, m_rA, b2Vec2);
	C(b2RopeJoint, m_rB, b2Vec2);
	F(b2RopeJoint, m_mass);
	F(b2RopeJoint, m_impulse);
	
	T(----------);
	
	T(b2FrictionJointDef);
	C(b2FrictionJointDef, localAnchorA, b2Vec2);
	C(b2FrictionJointDef, localAnchorB, b2Vec2);
	F(b2FrictionJointDef, maxForce);
	F(b2FrictionJointDef, maxTorque);
	
	T(----------);
	
	T(b2FrictionJoint);
	C(b2FrictionJoint, m_localAnchorA, b2Vec2);
	C(b2FrictionJoint, m_localAnchorB, b2Vec2);
	C(b2FrictionJoint, m_linearMass, b2Mat22);
	F(b2FrictionJoint, m_angularMass);
	C(b2FrictionJoint, m_linearImpulse, b2Vec2);
	F(b2FrictionJoint, m_angularImpulse);
	F(b2FrictionJoint, m_maxForce);
	F(b2FrictionJoint, m_maxTorque);
	
	T(----------);
	
	T(b2BroadPhase);
	I(b2BroadPhase, m_proxyCount);
	I(b2BroadPhase, m_moveBuffer);
	I(b2BroadPhase, m_moveCapacity);
	I(b2BroadPhase, m_moveCount);
	I(b2BroadPhase, m_pairCapacity);
	I(b2BroadPhase, m_pairCount);
	I(b2BroadPhase, m_queryProxyId);
	
	T(----------);
	
	T(b2ContactManager);
	C(b2ContactManager, m_contactList, b2Contact);
	C(b2ContactManager, m_broadPhase, b2BroadPhase);
	I(b2ContactManager, m_contactCount);
	
	T(----------);
	
	T(b2Jacobian);
	C(b2Jacobian, linearA, b2Vec2);
	F(b2Jacobian, angularA);
	C(b2Jacobian, linearB, b2Vec2);
	F(b2Jacobian, angularB);
	
	T(----------);
	
	T(b2ContactImpulse);
	F(b2ContactImpulse, normalImpulses[0]);
	F(b2ContactImpulse, normalImpulses[1]);
	F(b2ContactImpulse, tangentImpulses[0]);
	F(b2ContactImpulse, tangentImpulses[1]);
	
	T(----------);
	
	T(b2DistanceInput);
	C(b2DistanceInput, proxyA, b2DistanceProxy);
	C(b2DistanceInput, proxyB, b2DistanceProxy);
	C(b2DistanceInput, transformA, b2Transform);
	C(b2DistanceInput, transformB, b2Transform);
	B(b2DistanceInput, useRadii);
	
	T(----------);
	
	T(b2DistanceOutput);
	C(b2DistanceOutput, pointA, b2Vec2);
	C(b2DistanceOutput, pointB, b2Vec2);
	F(b2DistanceOutput, distance);
	I(b2DistanceOutput, iterations);
	
	T(----------);
	
	T(b2SimplexCache);
	F(b2SimplexCache, metric);
	U16(b2SimplexCache, count);
	U8(b2SimplexCache, indexA);
	U8(b2SimplexCache, indexB);
	
	T(----------);
	
	T(b2DistanceProxy);
	V(b2DistanceProxy, m_vertices, m_count);
	I(b2DistanceProxy, m_count);
	F(b2DistanceProxy, m_radius);

	T(----------);
	
	T(b2EdgeShape);
	C(b2EdgeShape, m_vertex0, b2Vec2);
	C(b2EdgeShape, m_vertex1, b2Vec2);
	C(b2EdgeShape, m_vertex2, b2Vec2);
	C(b2EdgeShape, m_vertex3, b2Vec2);
	B(b2EdgeShape, m_hasVertex0);
	B(b2EdgeShape, m_hasVertex3);

	T(----------);
	
	T(b2LoopShape);
	V(b2LoopShape, m_vertices, m_count);
	I(b2LoopShape, m_count);
	
	
	AS3_Trace(AS3_String("Creating and stepping a b2World in C++"));
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	bool doSleep = true;
	b2World* m_world = new b2World(gravity, doSleep);
	m_world->Step(0.05f, 15.0f, 15.0f);
	delete m_world;
	AS3_Trace(AS3_String("C++ b2World test done!"));
	
	AS3_Val core = b2Core();
	AS3_SetS(core, "emptyFunction", AS3F(emptyFunction));
	AS3_SetS(core, "testFunction", AS3F(testFunction));
	
	AS3_Trace(AS3_Ptr(&b2_aabbExtension));
	AS3_Trace(AS3_Ptr(&b2_aabbMultiplier));
	AS3_Trace(AS3_Ptr(&b2_linearSlop));
	AS3_Trace(AS3_Ptr(&b2_angularSlop));
	AS3_Trace(AS3_Ptr(&b2_polygonRadius));
	AS3_Trace(AS3_Ptr(&b2_maxSubSteps));
	AS3_Trace(AS3_Ptr(&b2_maxTOIContacts));
	AS3_Trace(AS3_Ptr(&b2_velocityThreshold));
	AS3_Trace(AS3_Ptr(&b2_maxLinearCorrection));
	AS3_Trace(AS3_Ptr(&b2_maxAngularCorrection));
	AS3_Trace(AS3_Ptr(&b2_maxTranslation));
	AS3_Trace(AS3_Ptr(&b2_maxTranslationSquared));
	AS3_Trace(AS3_Ptr(&b2_maxRotation));
	AS3_Trace(AS3_Ptr(&b2_maxRotationSquared));
	AS3_Trace(AS3_Ptr(&b2_contactBaumgarte));
	AS3_Trace(AS3_Ptr(&b2_timeToSleep));
	AS3_Trace(AS3_Ptr(&b2_linearSleepTolerance));
	AS3_Trace(AS3_Ptr(&b2_angularSleepTolerance));
	
	AS3_LibInit(core);
	
	return 0;
}