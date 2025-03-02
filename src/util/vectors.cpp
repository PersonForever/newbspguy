#include "vectors.h"
#include "mat4x4.h"
#include "util.h"

void vec3::operator-=(const vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void vec3::operator+=(const vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void vec3::operator*=(const vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void vec3::operator/=(const vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

void vec3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
}

void vec3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
}

void vec3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
}

void vec3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
}

vec3 operator*(float lhs, const vec3& rhs) {
	return vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

vec3 operator/(float lhs, const vec3& rhs) {
	return vec3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}

vec3 crossProduct(vec3 v1, vec3 v2)
{
	float x = v1.y * v2.z - v2.y * v1.z;
	float y = v2.x * v1.z - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;
	return vec3(x, y, z);
}

float crossProduct(vec2 v1, vec2 v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

vec3 crossProduct(const vec3& v1, const vec3& v2)
{
	float x = v1.y * v2.z - v2.y * v1.z;
	float y = v2.x * v1.z - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;
	return vec3(x, y, z);
}

float dotProduct(const vec3& v1, const vec3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float dotProduct(vec2 v1, vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float distanceToPlane(const vec3& point, const vec3& planeNormal, float planeDist) {
	return std::fabs(planeNormal.dot(point) - planeDist);
}

bool isPointInFace(const vec3& point, const std::vector<vec3>& faceVertices)
{
	vec3 v0 = faceVertices[1] - faceVertices[0];
	vec3 v1 = faceVertices[2] - faceVertices[0];
	vec3 v2 = point - faceVertices[0];

	float dot00 = v0.dot(v0);
	float dot01 = v0.dot(v1);
	float dot02 = v0.dot(v2);
	float dot11 = v1.dot(v1);
	float dot12 = v1.dot(v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}

void makeVectors(const vec3& angles, vec3& forward, vec3& right, vec3& up)
{
	mat4x4 rotMat;
	rotMat.loadIdentity();
	rotMat.rotateX(HL_PI * angles.x / 180.0f);
	rotMat.rotateY(HL_PI * angles.y / 180.0f);
	rotMat.rotateZ(HL_PI * angles.z / 180.0f);

	vec4 f = rotMat * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 r = rotMat * vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 u = rotMat * vec4(0.0f, 0.0f, 1.0f, 1.0f);

	forward = vec3(f.x, f.y, f.z);
	right = vec3(r.x, r.y, r.z);
	up = vec3(u.x, u.y, u.z);
}

vec3 vec3::normalize(float length) const
{
	if (std::fabs(x) < EPSILON2 && std::fabs(y) < EPSILON2 && std::fabs(z) < EPSILON2)
		return vec3();
	float d = length / sqrt((x * x) + (y * y) + (z * z));

	return vec3(x * d, y * d, z * d);
}

bool vec3::equal(vec3 to, float epsilon) const
{
	if (std::fabs(x - to.x) >= epsilon)
		return false;
	if (std::fabs(y - to.y) >= epsilon)
		return false;
	if (std::fabs(z - to.z) >= epsilon)
		return false;
	return true;
}

vec3 vec3::snap(float snapSize)
{
	if (snapSize < 0.00001f)
		return vec3(x, y, z);
	float _x = std::round(x / snapSize) * snapSize;
	float _y = std::round(y / snapSize) * snapSize;
	float _z = std::round(z / snapSize) * snapSize;

	return vec3(_x, _y, _z);
}

float fullnormalizeangle(float angle)
{
	float retval = fmod(angle, 360.0f);
	if (retval < -180.0f)
	{
		retval += 360.0f;
	}
	else if (retval > 180.0f)
	{
		retval -= 360.0f;
	}
	return retval;
}

vec3 vec3::normalize_angles() const
{
	return vec3(fullnormalizeangle(x), fullnormalizeangle(y), fullnormalizeangle(z));
}

vec3 vec3::swap_xz()
{
	return vec3(z, y, x);
}

vec3 vec3::abs()
{
	return vec3(std::fabs(x), std::fabs(y), std::fabs(z));
}

vec3 vec3::invert()
{
	return vec3(-x, -y, -z);
}

float vec3::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

float vec3::lengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

bool vec3::IsZero() const
{
	return (std::fabs(x) + std::fabs(y) + std::fabs(z)) < EPSILON;
}

std::string vec3::toString()
{
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

std::string vec3::toKeyvalueString(bool truncate, const std::string& suffix_x, const std::string& suffix_y, const std::string& suffix_z)
{
	std::string parts[3] = { std::to_string(x) ,std::to_string(y), std::to_string(z) };

	// remove trailing zeros to save some space
	for (int i = 0; i < 3; i++)
	{

		parts[i].erase(parts[i].find_last_not_of('0') + 1, std::string::npos);

		// strip dot if there's no fractional part
		if (parts[i][parts[i].size() - 1] == '.')
		{
			parts[i] = parts[i].substr(0, parts[i].size() - 1);
		}
		if (truncate)
		{
			size_t dotPosition = parts[i].find('.');
			if (dotPosition != std::string::npos) {
				parts[i] = parts[i].substr(0, dotPosition);
			}
		}

	}

	return parts[0] + suffix_x + parts[1] + suffix_y + parts[2] + suffix_z;
}

vec3 vec3::flip()
{
	return vec3(x, z, -y);
}

vec3 vec3::flipUV()
{
	return vec3(x, -z, y);
}

vec3 vec3::unflip()
{
	return flipUV();
}

vec3 vec3::unflipUV()
{
	return flip();
}


float vec3::size_test()
{
	return (x + y) * (z / 256.0f);
}

float vec3::sizeXY_test()
{
	return x + y;
}

float vec3::dist(vec3 to) const
{
	return sqrt(pow(x - to.x, 2.0f) + pow(y - to.y, 2.0f) + pow(z - to.z, 2.0f));
}

bool operator==(const vec2& v1, const vec2& v2)
{
	vec2 v = v1 - v2;
	if (std::fabs(v.x) >= EPSILON)
		return false;
	if (std::fabs(v.y) >= EPSILON)
		return false;
	return true;
}

bool operator!=(const vec2& v1, const vec2& v2)
{
	return std::fabs(v1.x - v2.x) >= EPSILON || std::fabs(v1.y - v2.y) >= EPSILON;
}

vec2 operator-(vec2 v1, const vec2& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return v1;
}

vec2 operator+(vec2 v1, const vec2& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return v1;
}

vec2 operator*(vec2 v1, const vec2& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	return v1;
}

vec2 operator/(vec2 v1, const vec2& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
	return v1;
}

vec2 operator-(vec2 v, float f)
{
	v.x -= f;
	v.y -= f;
	return v;
}

vec2 operator+(vec2 v, float f)
{
	v.x += f;
	v.y += f;
	return v;
}

vec2 operator*(vec2 v, float f)
{
	v.x *= f;
	v.y *= f;
	return v;
}

vec2 operator/(vec2 v, float f)
{
	v.x /= f;
	v.y /= f;
	return v;
}

void vec2::operator-=(const vec2& v)
{
	x -= v.x;
	y -= v.y;
}

void vec2::operator+=(const vec2& v)
{
	x += v.x;
	y += v.y;
}

void vec2::operator*=(const vec2& v)
{
	x *= v.x;
	y *= v.y;
}

void vec2::operator/=(const vec2& v)
{
	x /= v.x;
	y /= v.y;
}

void vec2::operator-=(float f)
{
	x -= f;
	y -= f;
}

void vec2::operator+=(float f)
{
	x += f;
	y += f;
}

void vec2::operator*=(float f)
{
	x *= f;
	y *= f;
}

void vec2::operator/=(float f)
{
	x /= f;
	y /= f;
}

float vec2::length()
{
	return sqrt((x * x) + (y * y));
}

vec2 vec2::normalize(float length)
{
	if (std::fabs(x) < EPSILON2 && std::fabs(y) < EPSILON2)
		return vec2();
	float d = length / sqrt((x * x) + (y * y));
	return vec2(x * d, y * d);
}

vec2 vec2::swap()
{
	return vec2(y, x);
}



bool operator==(const vec4& v1, const vec4& v2)
{
	vec4 v = v1 - v2;
	return std::fabs(v.x) < EPSILON && std::fabs(v.y) < EPSILON && std::fabs(v.z) < EPSILON && std::fabs(v.w) < EPSILON;
}


bool operator!=(const vec4& v1, const vec4& v2)
{
	return !(v1 == v2);
}


vec4 operator+(vec4 v1, const vec4& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	v1.w += v2.w;
	return v1;
}

vec4 operator+(vec4 v, float f)
{
	v.x += f;
	v.y += f;
	v.z += f;
	v.w += f;
	return v;
}

std::string vec4::toKeyvalueString(bool truncate, const std::string& suffix_x, const std::string& suffix_y, const std::string& suffix_z, const std::string& suffix_w)
{
	std::string parts[4] = { std::to_string(x) ,std::to_string(y), std::to_string(z) , std::to_string(w) };


	// remove trailing zeros to save some space
	for (int i = 0; i < 4; i++)
	{
		parts[i].erase(parts[i].find_last_not_of('0') + 1, std::string::npos);

		// strip dot if there's no fractional part
		if (parts[i][parts[i].size() - 1] == '.')
		{
			parts[i] = parts[i].substr(0, parts[i].size() - 1);
		}
		if (truncate)
		{
			size_t dotPosition = parts[i].find('.');
			if (dotPosition != std::string::npos) {
				parts[i] = parts[i].substr(0, dotPosition);
			}
		}

	}

	return parts[0] + suffix_x + parts[1] + suffix_y + parts[2] + suffix_z + parts[3] + suffix_w;
}

vec4 operator*(vec4 v1, const vec4& v2)
{
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
	v1.w *= v2.w;
	return v1;
}

vec4 operator*(vec4 v, float f)
{
	v.x *= f;
	v.y *= f;
	v.z *= f;
	v.w *= f;
	return v;
}



vec4 operator/(vec4 v1, const vec4& v2)
{
	v1.x /= v2.x;
	v1.y /= v2.y;
	v1.z /= v2.z;
	v1.w /= v2.w;
	return v1;
}

vec4 operator/(vec4 v, float f)
{
	v.x /= f;
	v.y /= f;
	v.z /= f;
	v.w /= f;
	return v;
}


vec4 operator-(vec4 v1, const vec4& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	v1.w -= v2.w;
	return v1;
}

vec4 operator-(vec4 v, float f)
{
	v.x -= f;
	v.y -= f;
	v.z -= f;
	v.w -= f;
	return v;
}

vec3 vec4::xyz()
{
	return vec3(x, y, z);
}

vec2 vec4::xy()
{
	return vec2(x, y);
}





bool VectorCompare(const vec3& v1, const vec3& v2, float epsilon)
{
	int		i;

	for (i = 0; i < 3; i++)
		if (std::fabs(v1[i] - v2[i]) > epsilon)
			return false;

	return true;
}



void AngleQuaternion(const vec3& angles, vec4& quaternion)
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	// FIXME: rescale the inputs to 1/2 angle
	angle = angles[2] * 0.5f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] * 0.5f;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0] * 0.5f;
	sr = sin(angle);
	cr = cos(angle);

	quaternion[0] = sr * cp * cy - cr * sp * sy; // X
	quaternion[1] = cr * sp * cy + sr * cp * sy; // Y
	quaternion[2] = cr * cp * sy - sr * sp * cy; // Z
	quaternion[3] = cr * cp * cy + sr * sp * sy; // W
}

void QuaternionSlerp(const vec4& p, vec4& q, float t, vec4& qt)
{
	int i;
	float omega, cosom, sinom, sclp, sclq;

	// decide if one of the quaternions is backwards
	float a = 0;
	float b = 0;
	for (i = 0; i < 4; i++)
	{
		a += (p[i] - q[i]) * (p[i] - q[i]);
		b += (p[i] + q[i]) * (p[i] + q[i]);
	}
	if (a > b)
	{
		for (i = 0; i < 4; i++)
		{
			q[i] = -q[i];
		}
	}

	cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

	if ((1.0f + cosom) > 0.00000001)
	{
		if ((1.0f - cosom) > 0.00000001)
		{
			omega = acos(cosom);
			sinom = sin(omega);
			sclp = sin((1.0f - t) * omega) / sinom;
			sclq = sin(t * omega) / sinom;
		}
		else
		{
			sclp = 1.0f - t;
			sclq = t;
		}
		for (i = 0; i < 4; i++)
		{
			qt[i] = sclp * p[i] + sclq * q[i];
		}
	}
	else
	{
		qt[0] = -p[1];
		qt[1] = p[0];
		qt[2] = -p[3];
		qt[3] = p[2];
		sclp = sin((1.0f - t) * 0.5f * HL_PI);
		sclq = sin(t * 0.5f * HL_PI);
		for (i = 0; i < 3; i++)
		{
			qt[i] = sclp * p[i] + sclq * qt[i];
		}
	}
}

void QuaternionMatrix(const vec4& quaternion, float(*matrix)[4])
{
	matrix[0][0] = 1.0f - 2.0f * quaternion[1] * quaternion[1] - 2.0f * quaternion[2] * quaternion[2];
	matrix[1][0] = 2.0f * quaternion[0] * quaternion[1] + 2.0f * quaternion[3] * quaternion[2];
	matrix[2][0] = 2.0f * quaternion[0] * quaternion[2] - 2.0f * quaternion[3] * quaternion[1];

	matrix[0][1] = 2.0f * quaternion[0] * quaternion[1] - 2.0f * quaternion[3] * quaternion[2];
	matrix[1][1] = 1.0f - 2.0f * quaternion[0] * quaternion[0] - 2.0f * quaternion[2] * quaternion[2];
	matrix[2][1] = 2.0f * quaternion[1] * quaternion[2] + 2.0f * quaternion[3] * quaternion[0];

	matrix[0][2] = 2.0f * quaternion[0] * quaternion[2] + 2.0f * quaternion[3] * quaternion[1];
	matrix[1][2] = 2.0f * quaternion[1] * quaternion[2] - 2.0f * quaternion[3] * quaternion[0];
	matrix[2][2] = 1.0f - 2.0f * quaternion[0] * quaternion[0] - 2.0f * quaternion[1] * quaternion[1];
}

void R_ConcatTransforms(float in1[][4], float in2[][4], float out[][4])
{
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];
}

void VectorScale(const vec3& v, float scale, vec3& out)
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
	out[2] = v[2] * scale;
}

float VectorNormalize(vec3& v)
{
	int		i;
	float	length;

	if (std::fabs(v[1] - 0.000215956f) < 0.0001f)
		i = 1;

	length = +0.0f;
	for (i = 0; i < 3; i++)
		length += v[i] * v[i];
	length = sqrt(length);
	if (std::fabs(length) < EPSILON)
		return 0.0f;

	for (i = 0; i < 3; i++)
		v[i] /= length;

	return length;
}

void VectorTransform(const vec3& in1, const float in2[3][4], vec3& out)
{
	out[0] = dotProduct(in1, vec3(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
	out[1] = dotProduct(in1, vec3(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
	out[2] = dotProduct(in1, vec3(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
}

int TextureAxisFromPlane(const BSPPLANE& pln, vec3& xv, vec3& yv)
{
	int             bestaxis;
	float           dot, best;
	int             i;

	best = 0;
	bestaxis = 0;

	for (i = 0; i < 6; i++)
	{
		dot = dotProduct(pln.vNormal, s_baseaxis[i * 3]);
		if (dot > best)
		{
			best = dot;
			bestaxis = i;
		}
	}

	xv = s_baseaxis[bestaxis * 3 + 1];
	yv = s_baseaxis[bestaxis * 3 + 2];

	return bestaxis;
}

int TextureAxisFromPlane(const vec3& pln, vec3& xv, vec3& yv)
{
	int             bestaxis;
	float           dot, best;
	int             i;

	best = 0;
	bestaxis = 0;

	for (i = 0; i < 6; i++)
	{
		dot = dotProduct(pln, s_baseaxis[i * 3]);
		if (dot > best)
		{
			best = dot;
			bestaxis = i;
		}
	}

	xv = s_baseaxis[bestaxis * 3 + 1];
	yv = s_baseaxis[bestaxis * 3 + 2];

	return bestaxis;
}

float VectorLength(const vec3& v)
{
	int		i;
	float	length = +0.0f;
	for (i = 0; i < 3; i++)
		length += v[i] * v[i];
	length = sqrt(length);		// FIXME

	return length;
}

float Q_rint(float in)
{
	return floor(in + 0.5f);
}

void mVectorMA(const vec3& va, float scale, const vec3& vb, vec3& vc)
{
	vc[0] = va[0] + scale * vb[0];
	vc[1] = va[1] + scale * vb[1];
	vc[2] = va[2] + scale * vb[2];
}

float _DotProduct(const vec3& v1, const vec3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void _VectorSubtract(const vec3& va, const vec3& vb, vec3& out)
{
	out[0] = va[0] - vb[0];
	out[1] = va[1] - vb[1];
	out[2] = va[2] - vb[2];
}

void _VectorAdd(const vec3& va, const vec3& vb, vec3& out)
{
	out[0] = va[0] + vb[0];
	out[1] = va[1] + vb[1];
	out[2] = va[2] + vb[2];
}

void _VectorCopy(const vec3& in, vec3& out)
{
	out[0] = in[0];
	out[1] = in[1];
	out[2] = in[2];
}

void _VectorScale(const vec3& v, float scale, vec3& out)
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
	out[2] = v[2] * scale;
}

void VectorInverse(vec3& v)
{
	v = -v;
}

void ClearBounds(vec3& mins, vec3& maxs)
{
	mins[0] = mins[1] = mins[2] = 99999.0f;
	maxs[0] = maxs[1] = maxs[2] = -99999.0f;
}

void AddPointToBounds(const vec3& v, vec3& mins, vec3& maxs)
{
	int		i;
	float	val;

	for (i = 0; i < 3; i++)
	{
		val = v[i];
		if (val < mins[i])
			mins[i] = val;
		if (val > maxs[i])
			maxs[i] = val;
	}
}

void AngleMatrix(const vec3& angles, float(*matrix)[4])
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	float multipl = (Q_PI * 2.0f / 360.0f);
	angle = angles[2] * multipl;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] * multipl;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0] * multipl;
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (Z * Y) * X
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[2][1] = sr * cp;
	matrix[0][2] = (cr * sp * cy + -sr * -sy);
	matrix[1][2] = (cr * sp * sy + -sr * cy);
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void AngleIMatrix(const vec3& angles, float matrix[3][4])
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;

	float multipl = (Q_PI * 2.0f / 360.0f);

	angle = angles[2] * multipl;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] * multipl;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0] * multipl;
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (Z * Y) * X
	matrix[0][0] = cp * cy;
	matrix[0][1] = cp * sy;
	matrix[0][2] = -sp;
	matrix[1][0] = sr * sp * cy + cr * -sy;
	matrix[1][1] = sr * sp * sy + cr * cy;
	matrix[1][2] = sr * cp;
	matrix[2][0] = (cr * sp * cy + -sr * -sy);
	matrix[2][1] = (cr * sp * sy + -sr * cy);
	matrix[2][2] = cr * cp;
	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void VectorRotate(const vec3& in1, const float in2[3][4], vec3& out)
{
	out[0] = dotProduct(in1, vec3(in2[0][0], in2[0][1], in2[0][2]));
	out[1] = dotProduct(in1, vec3(in2[1][0], in2[1][1], in2[1][2]));
	out[2] = dotProduct(in1, vec3(in2[2][0], in2[2][1], in2[2][2]));
}

void mCrossProduct(const vec3& v1, const vec3& v2, vec3& cross)
{
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

void VectorIRotate(const vec3& in1, const float in2[3][4], vec3& out)
{
	out[0] = in1[0] * in2[0][0] + in1[1] * in2[1][0] + in1[2] * in2[2][0];
	out[1] = in1[0] * in2[0][1] + in1[1] * in2[1][1] + in1[2] * in2[2][1];
	out[2] = in1[0] * in2[0][2] + in1[1] * in2[1][2] + in1[2] * in2[2][2];
}

float clamp(float val, float min, float max)
{
	if (val > max)
	{
		return max;
	}
	else if (val < min)
	{
		return min;
	}
	return val;
}

void VectorAngles(const vec3 & forward, vec3 & angles)
{
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / Q_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(forward[2], tmp) * 180 / Q_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}


unsigned char FixBounds(int i)
{
	if (i > 0xFF)
		return 0xFF;
	else if (i < 0x00)
		return 0x00;
	return (unsigned char)i;
}

unsigned char FixBounds(unsigned int i)
{
	if (i > 0xFF)
		return 0xFF;
	return (unsigned char)i;
}

unsigned char FixBounds(float i)
{
	if (i > (double)0xFF)
		return 0xFF;
	else if (i < (double)0x00)
		return 0x00;
	return (unsigned char)i;
}

unsigned char FixBounds(double i)
{
	if (i > (double)0xFF)
		return 0xFF;
	else if (i < (double)0x00)
		return 0x00;
	return (unsigned char)i;
}