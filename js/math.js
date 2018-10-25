const M00 = 0;
const M01 = 4;
const M02 = 8;
const M03 = 12;
const M10 = 1;
const M11 = 5;
const M12 = 9;
const M13 = 13;
const M20 = 2;
const M21 = 6;
const M22 = 10;
const M23 = 14;
const M30 = 3;
const M31 = 7;
const M32 = 11;
const M33 = 15;

const V0 = new Vector3(0, 0, 0);
const V1 = new Vector3(1, 1, 1);
const VX = new Vector3(1, 0, 0);
const VY = new Vector3(0, 1, 0);
const VZ = new Vector3(0, 0, 1);
const VNX = new Vector3(-1,  0,  0);
const VNY = new Vector3( 0, -1,  0);
const VNZ = new Vector3( 0,  0, -1);

const PIH = Math.PI / 2;
const PI = Math.PI;
const PI2 = 2 * Math.PI;

const RADDEG = 180 / PI;
const DEGRAD = PI / 180;

var __mjs_tmpMat1 = new Matrix4();
var __mjs_tmpMat2 = new Matrix4();
var __mjs_tmpQuat = new Quaternion();
var __mjs_tmpVec1 = new Vector3();
var __mjs_tmpVec2 = new Vector3();
var __mjs_tmpVec3 = new Vector3();

function Matrix4()
{
	this.val = new Array(16);
	
	this.set = function(b)
	{
		if(b instanceof Matrix4)
		{
			for(var i = 0; i < 16; i++)
				this.val[i] = b.val[i];
		}
		else if(b instanceof Quaternion)
		{
			var xs = b.x * 2,  ys = b.y * 2,  zs = b.z * 2;
			var wx = b.w * xs, wy = b.w * ys, wz = b.w * zs;
			var xx = b.x * xs, xy = b.x * ys, xz = b.x * zs;
			var yy = b.y * ys, yz = b.y * zs, zz = b.z * zs;
			
			this.val[M00] = 1 - yy - zz; this.val[M01] = xy - wz;     this.val[M02] = xz + wy;     this.val[M03] = 0;
			this.val[M10] = xy + wz;     this.val[M11] = 1 - xx - zz; this.val[M12] = yz - wx;     this.val[M13] = 0;
			this.val[M20] = xz - wy;     this.val[M21] = yz + wx;     this.val[M22] = 1 - xx - yy; this.val[M23] = 0;
			this.val[M30] = 0;           this.val[M31] = 0;           this.val[M32] = 0;           this.val[M33] = 1;
		}
		else if(b instanceof SimpleTransform)
		{
			this.setTranslation(b.translation).mul(b.rotation).mul(scale);
		}
		
		return this;
	};
	
	this.mul = function(b)
	{
		if(b instanceof Matrix4)
		{
			__mjs_tmpMat1.val[M00] = this.val[M00] * b.val[M00] + this.val[M01] * b.val[M10] + this.val[M02] * b.val[M20] + this.val[M03] * b.val[M30];
			__mjs_tmpMat1.val[M01] = this.val[M00] * b.val[M01] + this.val[M01] * b.val[M11] + this.val[M02] * b.val[M21] + this.val[M03] * b.val[M31];
			__mjs_tmpMat1.val[M02] = this.val[M00] * b.val[M02] + this.val[M01] * b.val[M12] + this.val[M02] * b.val[M22] + this.val[M03] * b.val[M32];
			__mjs_tmpMat1.val[M03] = this.val[M00] * b.val[M03] + this.val[M01] * b.val[M13] + this.val[M02] * b.val[M23] + this.val[M03] * b.val[M33];
			__mjs_tmpMat1.val[M10] = this.val[M10] * b.val[M00] + this.val[M11] * b.val[M10] + this.val[M12] * b.val[M20] + this.val[M13] * b.val[M30];
			__mjs_tmpMat1.val[M11] = this.val[M10] * b.val[M01] + this.val[M11] * b.val[M11] + this.val[M12] * b.val[M21] + this.val[M13] * b.val[M31];
			__mjs_tmpMat1.val[M12] = this.val[M10] * b.val[M02] + this.val[M11] * b.val[M12] + this.val[M12] * b.val[M22] + this.val[M13] * b.val[M32];
			__mjs_tmpMat1.val[M13] = this.val[M10] * b.val[M03] + this.val[M11] * b.val[M13] + this.val[M12] * b.val[M23] + this.val[M13] * b.val[M33];
			__mjs_tmpMat1.val[M20] = this.val[M20] * b.val[M00] + this.val[M21] * b.val[M10] + this.val[M22] * b.val[M20] + this.val[M23] * b.val[M30];
			__mjs_tmpMat1.val[M21] = this.val[M20] * b.val[M01] + this.val[M21] * b.val[M11] + this.val[M22] * b.val[M21] + this.val[M23] * b.val[M31];
			__mjs_tmpMat1.val[M22] = this.val[M20] * b.val[M02] + this.val[M21] * b.val[M12] + this.val[M22] * b.val[M22] + this.val[M23] * b.val[M32];
			__mjs_tmpMat1.val[M23] = this.val[M20] * b.val[M03] + this.val[M21] * b.val[M13] + this.val[M22] * b.val[M23] + this.val[M23] * b.val[M33];
			__mjs_tmpMat1.val[M30] = this.val[M30] * b.val[M00] + this.val[M31] * b.val[M10] + this.val[M32] * b.val[M20] + this.val[M33] * b.val[M30];
			__mjs_tmpMat1.val[M31] = this.val[M30] * b.val[M01] + this.val[M31] * b.val[M11] + this.val[M32] * b.val[M21] + this.val[M33] * b.val[M31];
			__mjs_tmpMat1.val[M32] = this.val[M30] * b.val[M02] + this.val[M31] * b.val[M12] + this.val[M32] * b.val[M22] + this.val[M33] * b.val[M32];
			__mjs_tmpMat1.val[M33] = this.val[M30] * b.val[M03] + this.val[M31] * b.val[M13] + this.val[M32] * b.val[M23] + this.val[M33] * b.val[M33];
			
			this.set(__mjs_tmpMat1);
		}
		else if(b instanceof Quaternion || b instanceof SimpleTransform)
		{
			this.mul(__mjs_tmpMat2.set(b));
		}
		else if(b instanceof Vector3)
		{
			this.mul(__mjs_tmpMat2.setScale(b));
		}
		
		return this;
	};
	
	this.add = function(vec)
	{
		return this.mul(__mjs_tmpMat2.setTranslation(vec));
	};
	
	this.sub = function(vec)
	{
		return this.mul(__mjs_tmpMat2.setTranslation(__mjs_tmpVec1.set(vec).scl(-1)));
	};
	
	this.setTranslation = function(vec)
	{
		this.val[M00] = 1; this.val[M01] = 0; this.val[M02] = 0;  this.val[M03] = vec.x;
		this.val[M10] = 0; this.val[M11] = 1; this.val[M12] = 0;  this.val[M13] = vec.y;
		this.val[M20] = 0; this.val[M21] = 0; this.val[M22] = 1;  this.val[M23] = vec.z;
		this.val[M30] = 0; this.val[M31] = 0; this.val[M32] = 0;  this.val[M33] = 1;
		return this;
	};
	
	this.setScale = function(vec)
	{
		this.val[M00] = vec.x; this.val[M01] = 0;     this.val[M02] = 0;     this.val[M03] = 0;
		this.val[M10] = 0;     this.val[M11] = vec.y; this.val[M12] = 0;     this.val[M13] = 0;
		this.val[M20] = 0;     this.val[M21] = 0;     this.val[M22] = vec.z; this.val[M23] = 0;
		this.val[M30] = 0;     this.val[M31] = 0;     this.val[M32] = 0;     this.val[M33] = 1;
		return this;
	};
	
	this.setOrthographic = function(left, right, bottom, top, near, far)
	{
		var rml = right - left;
		var tmb = top - bottom;
		var fmn = far - near;
		
		this.val[M00] = 2 / rml; this.val[M01] = 0;       this.val[M02] =  0;       this.val[M03] = -(right + left) / rml;
		this.val[M10] = 0;       this.val[M11] = 2 / tmb; this.val[M12] =  0;       this.val[M13] = -(top + bottom) / tmb;
		this.val[M20] = 0;       this.val[M21] = 0;       this.val[M22] = -2 / fmn; this.val[M23] = -(far + near)   / fmn;
		this.val[M30] = 0;       this.val[M31] = 0;       this.val[M32] =  0;       this.val[M33] = 1;
		return this;
	};
	
	this.setPerspective = function(near, far, fovy, aspectRatio)
	{
		var fd = (1 / Math.tan(fovy / 2));
		var a1 = (far + near) / (near - far);
		var a2 = (2 * far * near) / (near - far);
		
		this.val[M00] = fd / aspectRatio; this.val[M01] = 0;  this.val[M02] =  0;  this.val[M03] = 0;
		this.val[M10] = 0;                this.val[M11] = fd; this.val[M12] =  0;  this.val[M13] = 0;
		this.val[M20] = 0;                this.val[M21] = 0;  this.val[M22] =  a1; this.val[M23] = a2;
		this.val[M30] = 0;                this.val[M31] = 0;  this.val[M32] = -1;  this.val[M33] = 0;
		return this;
	};
	
	this.setView = function(position, direction, up)
	{
		var z = __mjs_tmpVec2.set(direction).nor(); //direction
		var x = __mjs_tmpVec3.set(z).crs(up).nor(); //right
		this.val[M00] =  x.x; this.val[M01] =  x.y; this.val[M02] =  x.z; this.val[M03] = 0;
		
		var y = __mjs_tmpVec3.crs(z).nor(); //corrected up
		this.val[M10] =  y.x; this.val[M11] =  y.y; this.val[M12] =  y.z; this.val[M13] = 0;
		this.val[M20] = -z.x; this.val[M21] = -z.y; this.val[M22] = -z.z; this.val[M23] = 0;
		this.val[M30] =  0;   this.val[M31] =  0;   this.val[M32] =  0;   this.val[M33] = 1;
		return this.sub(position);
	};
	
	this.calculateNormal = function(out) //make normal matrix from transform matrix
	{
		out.set(this);
		out.val[M03] = 0;
		out.val[M13] = 0;
		out.val[M23] = 0;
		out.inv();
		return out.tra();
	};
	
	this.idt = function()
	{
		this.val[M00] = 1; this.val[M01] = 0; this.val[M02] = 0; this.val[M03] = 0;
		this.val[M10] = 0; this.val[M11] = 1; this.val[M12] = 0; this.val[M13] = 0;
		this.val[M20] = 0; this.val[M21] = 0; this.val[M22] = 1; this.val[M23] = 0;
		this.val[M30] = 0; this.val[M31] = 0; this.val[M32] = 0; this.val[M33] = 1;
		return this;
	};
	
	this.inv = function()
	{
		var rdet = this.det();
		if(rdet == 0)
			return false;
		
		__mjs_tmpMat1.val[M00] = this.val[M12] * this.val[M23] * this.val[M31] - this.val[M13] * this.val[M22] * this.val[M31] + this.val[M13] * this.val[M21] * this.val[M32] -
				this.val[M11] * this.val[M23] * this.val[M32] - this.val[M12] * this.val[M21] * this.val[M33] + this.val[M11] * this.val[M22] * this.val[M33];
		__mjs_tmpMat1.val[M01] = this.val[M03] * this.val[M22] * this.val[M31] - this.val[M02] * this.val[M23] * this.val[M31] - this.val[M03] * this.val[M21] * this.val[M32] +
				this.val[M01] * this.val[M23] * this.val[M32] + this.val[M02] * this.val[M21] * this.val[M33] - this.val[M01] * this.val[M22] * this.val[M33];
		__mjs_tmpMat1.val[M02] = this.val[M02] * this.val[M13] * this.val[M31] - this.val[M03] * this.val[M12] * this.val[M31] + this.val[M03] * this.val[M11] * this.val[M32] -
				this.val[M01] * this.val[M13] * this.val[M32] - this.val[M02] * this.val[M11] * this.val[M33] + this.val[M01] * this.val[M12] * this.val[M33];
		__mjs_tmpMat1.val[M03] = this.val[M03] * this.val[M12] * this.val[M21] - this.val[M02] * this.val[M13] * this.val[M21] - this.val[M03] * this.val[M11] * this.val[M22] +
				this.val[M01] * this.val[M13] * this.val[M22] + this.val[M02] * this.val[M11] * this.val[M23] - this.val[M01] * this.val[M12] * this.val[M23];
		__mjs_tmpMat1.val[M10] = this.val[M13] * this.val[M22] * this.val[M30] - this.val[M12] * this.val[M23] * this.val[M30] - this.val[M13] * this.val[M20] * this.val[M32] +
				this.val[M10] * this.val[M23] * this.val[M32] + this.val[M12] * this.val[M20] * this.val[M33] - this.val[M10] * this.val[M22] * this.val[M33];
		__mjs_tmpMat1.val[M11] = this.val[M02] * this.val[M23] * this.val[M30] - this.val[M03] * this.val[M22] * this.val[M30] + this.val[M03] * this.val[M20] * this.val[M32] -
				this.val[M00] * this.val[M23] * this.val[M32] - this.val[M02] * this.val[M20] * this.val[M33] + this.val[M00] * this.val[M22] * this.val[M33];
		__mjs_tmpMat1.val[M12] = this.val[M03] * this.val[M12] * this.val[M30] - this.val[M02] * this.val[M13] * this.val[M30] - this.val[M03] * this.val[M10] * this.val[M32] +
				this.val[M00] * this.val[M13] * this.val[M32] + this.val[M02] * this.val[M10] * this.val[M33] - this.val[M00] * this.val[M12] * this.val[M33];
		__mjs_tmpMat1.val[M13] = this.val[M02] * this.val[M13] * this.val[M20] - this.val[M03] * this.val[M12] * this.val[M20] + this.val[M03] * this.val[M10] * this.val[M22] -
				this.val[M00] * this.val[M13] * this.val[M22] - this.val[M02] * this.val[M10] * this.val[M23] + this.val[M00] * this.val[M12] * this.val[M23];
		__mjs_tmpMat1.val[M20] = this.val[M11] * this.val[M23] * this.val[M30] - this.val[M13] * this.val[M21] * this.val[M30] + this.val[M13] * this.val[M20] * this.val[M31] -
				this.val[M10] * this.val[M23] * this.val[M31] - this.val[M11] * this.val[M20] * this.val[M33] + this.val[M10] * this.val[M21] * this.val[M33];
		__mjs_tmpMat1.val[M21] = this.val[M03] * this.val[M21] * this.val[M30] - this.val[M01] * this.val[M23] * this.val[M30] - this.val[M03] * this.val[M20] * this.val[M31] +
				this.val[M00] * this.val[M23] * this.val[M31] + this.val[M01] * this.val[M20] * this.val[M33] - this.val[M00] * this.val[M21] * this.val[M33];
		__mjs_tmpMat1.val[M22] = this.val[M01] * this.val[M13] * this.val[M30] - this.val[M03] * this.val[M11] * this.val[M30] + this.val[M03] * this.val[M10] * this.val[M31] -
				this.val[M00] * this.val[M13] * this.val[M31] - this.val[M01] * this.val[M10] * this.val[M33] + this.val[M00] * this.val[M11] * this.val[M33];
		__mjs_tmpMat1.val[M23] = this.val[M03] * this.val[M11] * this.val[M20] - this.val[M01] * this.val[M13] * this.val[M20] - this.val[M03] * this.val[M10] * this.val[M21] +
				this.val[M00] * this.val[M13] * this.val[M21] + this.val[M01] * this.val[M10] * this.val[M23] - this.val[M00] * this.val[M11] * this.val[M23];
		__mjs_tmpMat1.val[M30] = this.val[M12] * this.val[M21] * this.val[M30] - this.val[M11] * this.val[M22] * this.val[M30] - this.val[M12] * this.val[M20] * this.val[M31] +
				this.val[M10] * this.val[M22] * this.val[M31] + this.val[M11] * this.val[M20] * this.val[M32] - this.val[M10] * this.val[M21] * this.val[M32];
		__mjs_tmpMat1.val[M31] = this.val[M01] * this.val[M22] * this.val[M30] - this.val[M02] * this.val[M21] * this.val[M30] + this.val[M02] * this.val[M20] * this.val[M31] -
				this.val[M00] * this.val[M22] * this.val[M31] - this.val[M01] * this.val[M20] * this.val[M32] + this.val[M00] * this.val[M21] * this.val[M32];
		__mjs_tmpMat1.val[M32] = this.val[M02] * this.val[M11] * this.val[M30] - this.val[M01] * this.val[M12] * this.val[M30] - this.val[M02] * this.val[M10] * this.val[M31] +
				this.val[M00] * this.val[M12] * this.val[M31] + this.val[M01] * this.val[M10] * this.val[M32] - this.val[M00] * this.val[M11] * this.val[M32];
		__mjs_tmpMat1.val[M33] = this.val[M01] * this.val[M12] * this.val[M20] - this.val[M02] * this.val[M11] * this.val[M20] + this.val[M02] * this.val[M10] * this.val[M21] -
				this.val[M00] * this.val[M12] * this.val[M21] - this.val[M01] * this.val[M10] * this.val[M22] + this.val[M00] * this.val[M11] * this.val[M22];
		
		for(var i = 0; i < 16; i++)
			this.val[i] = __mjs_tmpMat1.val[i] / rdet;
		
		return true;
	};
	
	this.tra = function()
	{
		__mjs_tmpMat1.val[M00] = this.val[M00];
		__mjs_tmpMat1.val[M01] = this.val[M10];
		__mjs_tmpMat1.val[M02] = this.val[M20];
		__mjs_tmpMat1.val[M03] = this.val[M30];
		__mjs_tmpMat1.val[M10] = this.val[M01];
		__mjs_tmpMat1.val[M11] = this.val[M11];
		__mjs_tmpMat1.val[M12] = this.val[M21];
		__mjs_tmpMat1.val[M13] = this.val[M31];
		__mjs_tmpMat1.val[M20] = this.val[M02];
		__mjs_tmpMat1.val[M21] = this.val[M12];
		__mjs_tmpMat1.val[M22] = this.val[M22];
		__mjs_tmpMat1.val[M23] = this.val[M32];
		__mjs_tmpMat1.val[M30] = this.val[M03];
		__mjs_tmpMat1.val[M31] = this.val[M13];
		__mjs_tmpMat1.val[M32] = this.val[M23];
		__mjs_tmpMat1.val[M33] = this.val[M33];
		return this.set(__mjs_tmpMat1);
	};
	
	this.rotate = function(axis, rad)
	{
		return this.mul(__mjs_tmpQuat.setRotation(axis, rad));
	};
	
	this.det = function()
	{
		return this.val[M30] * this.val[M21] * this.val[M12] * this.val[M03] - this.val[M20] * this.val[M31] * this.val[M12] * this.val[M03] -
				this.val[M30] * this.val[M11] * this.val[M22] * this.val[M03] + this.val[M10] * this.val[M31] * this.val[M22] * this.val[M03] +
				this.val[M20] * this.val[M11] * this.val[M32] * this.val[M03] - this.val[M10] * this.val[M21] * this.val[M32] * this.val[M03] -
				this.val[M30] * this.val[M21] * this.val[M02] * this.val[M13] + this.val[M20] * this.val[M31] * this.val[M02] * this.val[M13] +
				this.val[M30] * this.val[M01] * this.val[M22] * this.val[M13] - this.val[M00] * this.val[M31] * this.val[M22] * this.val[M13] -
				this.val[M20] * this.val[M01] * this.val[M32] * this.val[M13] + this.val[M00] * this.val[M21] * this.val[M32] * this.val[M13] +
				this.val[M30] * this.val[M11] * this.val[M02] * this.val[M23] - this.val[M10] * this.val[M31] * this.val[M02] * this.val[M23] -
				this.val[M30] * this.val[M01] * this.val[M12] * this.val[M23] + this.val[M00] * this.val[M31] * this.val[M12] * this.val[M23] +
				this.val[M10] * this.val[M01] * this.val[M32] * this.val[M23] - this.val[M00] * this.val[M11] * this.val[M32] * this.val[M23] -
				this.val[M20] * this.val[M11] * this.val[M02] * this.val[M33] + this.val[M10] * this.val[M21] * this.val[M02] * this.val[M33] +
				this.val[M20] * this.val[M01] * this.val[M12] * this.val[M33] - this.val[M00] * this.val[M21] * this.val[M12] * this.val[M33] -
				this.val[M10] * this.val[M01] * this.val[M22] * this.val[M33] + this.val[M00] * this.val[M11] * this.val[M22] * this.val[M33];
	};
	
	this.idt();
}

function Quaternion()
{
	this.set = function(x, y, z, w)
	{
		if(x instanceof Quaternion)
		{
			this.x = x.x;
			this.y = x.y;
			this.z = x.z;
			this.w = x.w;
		}
		else
		{
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}
		
		return this;
	};
	
	this.setRotation = function(axis, rad)
	{
		var d = axis.len2();
		if(d == 0) return idt();
		d = Math.sqrt(d);
		
		var ang = rad < 0 ? PI2 - (-rad % PI2) : (rad % PI2);
		var fsin = Math.sin(ang / 2);
		var fcos = Math.cos(ang / 2);
		
		return this.set(axis.x * fsin / d, axis.y * fsin / d, axis.z * fsin / d, fcos).nor();
	};
	
	this.idt = function()
	{
		return this.set(0, 0, 0, 1);
	};
	
	this.nor = function()
	{
		var len = this.x * this.x + this.y * this.y + this.z * this.z + this.w * this.w;
		if(len != 0 && len != 1)
		{
			len = Math.sqrt(len);
			this.w /= len;
			this.x /= len;
			this.y /= len;
			this.z /= len;
		}
		
		return this;
	};
	
	this.idt();
}

function Vector3(x, y, z)
{
	this.set = function(x, y, z)
	{
		if(x instanceof Vector3)
		{
			this.x = x.x;
			this.y = x.y;
			this.z = x.z;
		}
		else
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}
		
		return this;
	}
	
	this.mul = function(mat)
	{
		return this.set(this.x * mat.val[M00] + this.y * mat.val[M01] + this.z * mat.val[M02] + mat.val[M03],
				this.x * mat.val[M10] + this.y * mat.val[M11] + this.z * mat.val[M12] + mat.val[M13],
				this.x * mat.val[M20] + this.y * mat.val[M21] + this.z * mat.val[M22] + mat.val[M23]);
	}
	
	this.add = function(vec)
	{
		this.x += vec.x;
		this.y += vec.y;
		this.z += vec.z;
		return this;
	}
	
	this.sub = function(vec)
	{
		this.x -= vec.x;
		this.y -= vec.y;
		this.z -= vec.z;
		return this;
	}
	
	this.scl = function(scalar)
	{
		this.x *= scalar;
		this.y *= scalar;
		this.z *= scalar;
		return this;
	}
	
	this.nor = function()
	{
		var len = this.len2();
		if(len != 0 && len != 1)
		{
			len = Math.sqrt(len);
			this.scl(1 / len);
		}
		
		return this;
	}
	
	this.crs = function(vec)
	{
		__mjs_tmpVec1.x = this.y * vec.z - this.z * vec.y;
		__mjs_tmpVec1.y = this.z * vec.x - this.x * vec.z;
		__mjs_tmpVec1.z = this.x * vec.y - this.y * vec.x;
		return this.set(__mjs_tmpVec1);
	}
	
	this.rotate = function(axis, rad)
	{
		return this.mul(__mjs_tmpMat1.set(__mjs_tmpQuat.setRotation(axis, rad)));
	}
	
	this.len2 = function()
	{
		return this.x * this.x + this.y * this.y + this.z * this.z;
	}
	
	this.len = function()
	{
		return Math.sqrt(this.len2());
	}
	
	this.dot = function(vec)
	{
		return this.x * vec.x + this.y * vec.y + this.z * vec.z;
	}
	
	this.set(x ? x : 0, y ? y : 0, z ? z : 0);
}

function SimpleTransform()
{
	this.translation = new Vector3();
	this.rotation = new Quaternion();
	this.scale = new Vector3();
	
	this.set = function(st)
	{
		this.translation.set(st.translation);
		this.rotation.set(st.rotation);
		this.scale.set(st.scale);
	}
}
