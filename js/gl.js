const COLOR_CLEAR   = new Color(0, 0, 0, 0);
const COLOR_BLACK   = new Color(0, 0, 0, 1);
const COLOR_RED     = new Color(1, 0, 0, 1);
const COLOR_GREEN   = new Color(0, 1, 0, 1);
const COLOR_BLUE    = new Color(0, 0, 1, 1);
const COLOR_YELLOW  = new Color(1, 1, 0, 1);
const COLOR_CYAN    = new Color(0, 1, 1, 1);
const COLOR_MAGENTA = new Color(1, 0, 1, 1);
const COLOR_WHITE   = new Color(1, 1, 1, 1);

var deltaTime = 0;

var __gljs_tmpVec = new Vector3();
var __gljs_prvTime = 0;

function Color(r, g, b, a)
{
	this.set = function(r, g, b, a)
	{
		if(r instanceof Color)
		{
			this.r = r.r;
			this.g = r.g;
			this.b = r.b;
			this.a = r.a;
		}
		else
		{
			this.r = r;
			this.g = g;
			this.b = b;
			this.a = a;
		}
		
		return this;
	}
	
	this.set(r, g, b, a);
}

function Camera()
{
	this.speed = 0.05;
	this.sensitivity = 0.0025;
	
	this.keyForward = "w";
	this.keyBackward = "s";
	this.keyLeft = "a";
	this.keyRight = "d";
	this.keyUp = " ";
	this.keyDown = "shift";
	
	this.position = new Vector3(V0);
	this.direction = new Vector3(VNZ);
	this.up = new Vector3(VY);
	
	this.projection = new Matrix4();
	this.view = new Matrix4();
	
	this.firstFrame = true;
	
	this.setFPSYaw = function(yaw)
	{
		while(yaw >= PI2) yaw -= PI2;
		while(yaw < 0)    yaw += PI2;
		this.yaw = yaw;
	};
	
	this.setFPSPitch = function(pitch)
	{
		var min = -PIH + 0.1;
		var max = PIH - 0.1;
		
		if(pitch < min)
			this.pitch = min;
		else if(pitch > max)
			this.pitch = max;
		else
			this.pitch = pitch;
	};
	
	this.setFPSLookAt = function(at)
	{
		at = __gljs_tmpVec.set(this.position).sub(this.position).nor();
	
		this.setFPSYaw(-PIH + Math.atan2(-at.z, at.x));
		this.setFPSPitch(Math.atan2(at.y, Math.sqrt(at.x * at.x + at.z * at.z)));
	};
	
	this.calculateFPSView = function(keyState, mouseX, mouseY)
	{
		if(!this.firstFrame)
		{
			var dx = (mouseX - this.prvx) * this.sensitivity;
			var dy = (mouseY - this.prvy) * this.sensitivity;
			
			this.setFPSYaw(this.yaw - dx);
			this.setFPSPitch(this.pitch - dy);
			
			this.up.set(VY);
			this.direction.set(VNZ);
			this.direction.rotate(this.up, this.yaw);
			this.direction.rotate(__gljs_tmpVec.set(this.direction).crs(this.up), this.pitch);
		}
		else
		{
			this.yaw = 0;
			this.pitch = 0;
			this.firstFrame = false;
		}
		
		this.prvx = mouseX;
		this.prvy = mouseY;
		
		var deltaSpeed = getDeltaSpeed(this.speed);
		
		if(keyState[this.keyForward])
			this.position.add(__gljs_tmpVec.set(this.direction.x, 0, this.direction.z).nor().scl(deltaSpeed));
		
		if(keyState[this.keyBackward])
			this.position.sub(__gljs_tmpVec.set(this.direction.x, 0, this.direction.z).nor().scl(deltaSpeed));
		
		if(keyState[this.keyLeft])
			this.position.sub(__gljs_tmpVec.set(this.direction).crs(VY).nor().scl(deltaSpeed));
		
		if(keyState[this.keyRight])
			this.position.add(__gljs_tmpVec.set(this.direction).crs(VY).nor().scl(deltaSpeed));
		
		if(keyState[this.keyUp])
			this.position.y += deltaSpeed;
		
		if(keyState[this.keyDown])
			this.position.y -= deltaSpeed;
	};
	
	this.calculateMatrix = function(out)
	{
		this.view.setView(this.position, this.direction, this.up);
		return out.set(this.projection).mul(this.view);
	};
}

function Vertex(position, normal, u, v)
{
	this.position = position ? position : new Vector3();
	this.normal = normal ? normal : new Vector3();
	this.u = u ? u : 0;
	this.v = v ? v : 0;
}

function Texture(gl, file, wrapS, wrapT, minFilter, magFilter)
{
	var dataLoad = new Uint8Array( //checkerboard loading pattern while waiting for file to download
	[
		255, 255, 255, 255,
		  0,   0,   0, 255,
		  0,   0,   0, 255,
		255, 255, 255, 255
	]);
	
	this.id = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, this.id);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 2, 2, 0, gl.RGBA, gl.UNSIGNED_BYTE, dataLoad);
	
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, wrapS ? wrapS : gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, wrapT ? wrapT : gl.REPEAT);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
	
	var thiz = this;
	var data = new Image();
	data.onload = function()
	{
		thiz.width = data.width;
		thiz.height = data.height;
		
		gl.bindTexture(gl.TEXTURE_2D, thiz.id);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, data.width, data.height, 0, gl.RGBA, gl.UNSIGNED_BYTE, data);
		gl.generateMipmap(gl.TEXTURE_2D);
		
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, minFilter ? minFilter : gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, magFilter ? magFilter : gl.LINEAR);
	}
	
	data.src = file;
	
	this.bind = function(active)
	{
		gl.activeTexture(gl.TEXTURE0 + active);
		gl.bindTexture(gl.TEXTURE_2D, this.id);
	};
	
	this.dispose = function()
	{
		gl.deleteTexture(this.id);
	}
}

function Material()
{
	this.ambientColor = new Color(COLOR_WHITE);
	this.ambientTexture = null;
	this.ambientTextureEnable = false;
	
	this.diffuseColor = new Color(COLOR_WHITE);
	this.diffuseTexture = null;
	this.diffuseTextureEnable = false;
	
	this.specularColor = new Color(0.5, 0.5, 0.5, 1);
	this.specularTexture = null;
	this.specularTextureEnable = false;
	this.specularShine = 32;
}

function Mesh(gl, vertices, indices)
{
	this.VAO = gl.createVertexArray();
	this.VBO = gl.createBuffer();
	this.EBO = gl.createBuffer();
	
	var floatSize = 4;
	var vertexAttCnt = 8;
	var vertexSize = floatSize * vertexAttCnt;
	
	var vertexData = new Array(vertices.length * vertexAttCnt);
	for(var i = 0; i < vertices.length; i++)
	{
		var vi = i * vertexAttCnt;
		vertexData[vi    ] = vertices[i].position.x;
		vertexData[vi + 1] = vertices[i].position.y;
		vertexData[vi + 2] = vertices[i].position.z;
		vertexData[vi + 3] = vertices[i].normal.x;
		vertexData[vi + 4] = vertices[i].normal.y;
		vertexData[vi + 5] = vertices[i].normal.z;
		vertexData[vi + 6] = vertices[i].u;
		vertexData[vi + 7] = vertices[i].v;
	}
	
	gl.bindVertexArray(this.VAO);
	
	gl.bindBuffer(gl.ARRAY_BUFFER, this.VBO);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexData), gl.STATIC_DRAW);
	this.vlength = vertices.length;
	
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.EBO);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint32Array(indices), gl.STATIC_DRAW);
	this.ilength = indices.length;
	
	gl.vertexAttribPointer(0, 3, gl.FLOAT, false, vertexSize, 0);
	gl.vertexAttribPointer(1, 3, gl.FLOAT, false, vertexSize, 3 * floatSize);
	gl.vertexAttribPointer(2, 2, gl.FLOAT, false, vertexSize, 6 * floatSize);
	
	gl.enableVertexAttribArray(0);
	gl.enableVertexAttribArray(1);
	gl.enableVertexAttribArray(2);
	
	
	this.bind = function()
	{
		gl.bindVertexArray(this.VAO);
	}
	
	this.dispose = function()
	{
		gl.deleteBuffer(this.EBO);
		gl.deleteBuffer(this.VBO);
		gl.deleteVertexArray(this.VAO);
	}
}

function MatMesh(material, mesh)
{
	this.material = material;
	this.mesh = mesh;
}

function Model(...matmesh)
{
	this.data = matmesh ? matmesh : [];
	this.transform = new Matrix4();
}

function Shader(gl, vertex, fragment, debug)
{
	var vertexShader = gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShader, vertex);
	gl.compileShader(vertexShader);
	
	var fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShader, fragment);
	gl.compileShader(fragmentShader);
	
	if(debug)
	{
		console.log("VERTEX SHADER COMPILATION LOG:");
		console.log(gl.getShaderInfoLog(vertexShader));
		
		console.log("FRAGMENT SHADER COMPILATION LOG:");
		console.log(gl.getShaderInfoLog(fragmentShader));
	}
	
	this.id = gl.createProgram();
	gl.attachShader(this.id, vertexShader);
	gl.attachShader(this.id, fragmentShader);
	gl.linkProgram(this.id);
	gl.deleteShader(vertexShader);
	gl.deleteShader(fragmentShader);
	
	this.use = function()
	{
		gl.useProgram(this.id);
	}
	
	this.getUniform = function(name)
	{
		return gl.getUniformLocation(this.id, name);
	}
	
	this.setUniform1b = function(name, b)
	{
		if(typeof name == "string")
			gl.uniform1i(this.getUniform(name), b ? 1 : 0);
		else
			gl.uniform1i(name, b ? 1 : 0);
	}
	
	this.setUniform1i = function(name, i)
	{
		if(typeof name == "string")
			gl.uniform1i(this.getUniform(name), i);
		else
			gl.uniform1i(name, i);
	}
	
	this.setUniform1f = function(name, f)
	{
		if(typeof name == "string")
			gl.uniform1f(this.getUniform(name), f);
		else
			gl.uniform1f(name, f);
	}
	
	this.setUniform3f = function(name, vec)
	{
		if(typeof name == "string")
			gl.uniform3f(this.getUniform(name), vec.x, vec.y, vec.z);
		else
			gl.uniform3f(name, vec.x, vec.y, vec.z);
	}
	
	this.setUniform4f = function(name, col)
	{
		if(typeof name == "string")
			gl.uniform4f(this.getUniform(name), col.r, col.g, col.b, col.a);
		else
			gl.uniform4f(name, col.r, col.g, col.b, col.a);
	}
	
	this.setUniform4x4f = function(name, mat)
	{
		if(typeof name == "string")
			gl.uniformMatrix4fv(this.getUniform(name), false, mat.val);
		else
			gl.uniformMatrix4fv(name, false, mat.val);
	}
	
	this.dispose = function()
	{
		gl.deleteProgram(this.id);
	}
}

function DirectionalLight()
{
	this.direction = new Vector3(VNY);
	this.ambient = new Vector3();
	this.diffuse = new Vector3(V1);
	this.specular = new Vector3();
}

function PointLight()
{
	this.position = new Vector3();
	this.ambient = new Vector3();
	this.diffuse = new Vector3(V1);
	this.specular = new Vector3();
	
	this.attenuationConstant = 1;
	this.attenuationLinear = 0;
	this.attenuationQuadratic = 0;
}

function SpotLight()
{
	this.position = new Vector3();
	this.direction = new Vector3();
	this.ambient = new Vector3();
	this.diffuse = new Vector3(V1);
	this.specular = new Vector3();
	
	this.attenuationConstant = 1;
	this.attenuationLinear = 0;
	this.attenuationQuadratic = 0;
	
	this.innerCutOff = Math.cos(7.5 * DEGRAD);
	this.outerCutOff = Math.cos(15 * DEGRAD);
}

function createDefaultShader(gl, maxPointLights = 8, maxSpotLights = 8)
{
	var vertexShaderSource =
	"#version 300 es\n" +
	"precision highp float;\n" +
	"\n" +
	"layout (location = 0) in vec3 aPos;\n" +
	"layout (location = 1) in vec3 aNormal;\n" +
	"layout (location = 2) in vec2 aTexuv;\n" +
	"\n" +
	"out vec3 vpos;\n" +
	"out vec3 normal;\n" +
	"out vec2 texuv;\n" +
	"\n" +
	"uniform mat4 camMat, norMat, traMat;\n" +
	"\n" +
	"void main()\n" +
	"{\n" +
	"	vec4 vpos4 = traMat * vec4(aPos, 1.f);\n" +
	"	\n" +
	"	vpos = vpos4.xyz;\n" +
	"	normal = normalize(mat3(norMat) * aNormal);\n" +
	"	texuv = aTexuv;\n" +
	"	\n" +
	"	gl_Position = camMat * vpos4;\n" +
	"}";
	
	var fragmentShaderSource =
	"#version 300 es\n" +
	"precision highp float;\n" +
	"\n" +
	"#define POINTLIGHTS " + maxPointLights + "\n" +
	"#define SPOTLIGHTS " + maxSpotLights + "\n" +
	"\n" +
	"struct Material\n" +
	"{\n" +
	"	vec4 ambCol;\n" +
	"	sampler2D ambTex;\n" +
	"	bool ambTexEn;\n" +
	"	\n" +
	"	vec4 diffCol;\n" +
	"	sampler2D diffTex;\n" +
	"	bool diffTexEn;\n" +
	"	\n" +
	"	vec4 specCol;\n" +
	"	sampler2D specTex;\n" +
	"	bool specTexEn;\n" +
	"	float specShine;\n" +
	"};\n" +
	"\n" +
	"struct DirectionalLight\n" +
	"{\n" +
	"	bool en;\n" +
	"	vec3 dir;\n" +
	"	\n" +
	"	vec3 amb;\n" +
	"	vec3 diff;\n" +
	"	vec3 spec;\n" +
	"};\n" +
	"\n" +
	"struct PointLight\n" +
	"{\n" +
	"	bool en;\n" +
	"	vec3 pos;\n" +
	"	\n" +
	"	vec3 amb;\n" +
	"	vec3 diff;\n" +
	"	vec3 spec;\n" +
	"	\n" +
	"	float attConst;\n" +
	"	float attLin;\n" +
	"	float attQuad;\n" +
	"};\n" +
	"\n" +
	"struct SpotLight\n" +
	"{\n" +
	"	bool en;\n" +
	"	vec3 pos;\n" +
	"	vec3 dir;\n" +
	"	\n" +
	"	vec3 amb;\n" +
	"	vec3 diff;\n" +
	"	vec3 spec;\n" +
	"	\n" +
	"	float attConst;\n" +
	"	float attLin;\n" +
	"	float attQuad;\n" +
	"	\n" +
	"	float inner;\n" +
	"	float outer;\n" +
	"};\n" +
	"\n" +
	"in vec3 vpos;\n" +
	"in vec3 normal;\n" +
	"in vec2 texuv;\n" +
	"\n" +
	"out vec4 fragColor;\n" +
	"\n" +
	"uniform Material mat;\n" +
	"uniform DirectionalLight dirLight;\n" +
	"uniform PointLight pointLights[POINTLIGHTS];\n" +
	"uniform SpotLight spotLights[SPOTLIGHTS];\n" +
	"uniform vec3 camPos;\n" +
	"\n" +
	"vec4 calcAmbient(vec3 vamb)\n" +
	"{\n" +
	"	vec4 ret = vec4(vamb, 1.f) * mat.ambCol;\n" +
	"	if(mat.ambTexEn)\n" +
	"		ret *= texture(mat.ambTex, texuv);\n" +
	"	\n" +
	"	return ret;\n" +
	"}\n" +
	"\n" +
	"vec4 calcDiffuse(vec3 vdiff, vec3 lightDir)\n" +
	"{\n" +
	"	float fdiff = max(dot(normal, lightDir), 0.f);\n" +
	"	vec4 ret = vec4(vdiff, 1.f) * mat.diffCol * fdiff;\n" +
	"	if(mat.diffTexEn)\n" +
	"		ret *= texture(mat.diffTex, texuv);\n" +
	"	\n" +
	"	return ret;\n" +
	"}\n" +
	"\n" +
	"vec4 calcSpecular(vec3 vspec, vec3 lightDir)\n" +
	"{\n" +
	"	vec3 camDir = normalize(camPos - vpos);\n" +
	"	vec3 reflectDir = reflect(-lightDir, normal);\n" +
	"	float fspec = pow(max(dot(camDir, reflectDir), 0.f), mat.specShine);\n" +
	"	vec4 ret = vec4(vspec, 1.f) * mat.specCol * fspec;\n" +
	"	if(mat.specTexEn)\n" +
	"		ret *= texture(mat.specTex, texuv);\n" +
	"	\n" +
	"	return ret;\n" +
	"}\n" +
	"\n" +
	"float calcAttenuation(vec3 lightPos, float attConst, float attLin, float attQuad)\n" +
	"{\n" +
	"	float distance = length(lightPos - vpos);\n" +
	"	return 1.f / (attConst + attLin * distance + attQuad * distance * distance);\n" +
	"}\n" +
	"\n" +
	"vec4 calcDirectionalLight()\n" +
	"{\n" +
	"	vec3 lightDir = normalize(-dirLight.dir);\n" +
	"	\n" +
	"	vec4 ambient = calcAmbient(dirLight.amb);\n" +
	"	vec4 diffuse = calcDiffuse(dirLight.diff, lightDir);\n" +
	"	vec4 specular = calcSpecular(dirLight.spec, lightDir);\n" +
	"	\n" +
	"	return ambient + diffuse + specular;\n" +
	"}\n" +
	"\n" +
	"vec4 calcPointLight(int i)\n" +
	"{\n" +
	"	PointLight light = pointLights[i];\n" +
	"	vec3 lightDir = normalize(light.pos - vpos);\n" +
	"	\n" +
	"	vec4 ambient = calcAmbient(light.amb);\n" +
	"	vec4 diffuse = calcDiffuse(light.diff, lightDir);\n" +
	"	vec4 specular = calcSpecular(light.spec, lightDir);\n" +
	"	\n" +
	"	float attenuation = calcAttenuation(light.pos, light.attConst, light.attLin, light.attQuad);\n" +
	"	ambient *= attenuation;\n" +
	"	diffuse *= attenuation;\n" +
	"	specular *= attenuation;\n" +
	"	\n" +
	"	return ambient + diffuse + specular;\n" +
	"}\n" +
	"\n" +
	"vec4 calcSpotLight(int i)\n" +
	"{\n" +
	"	SpotLight light = spotLights[i];\n" +
	"	vec3 lightDir = normalize(light.pos - vpos);\n" +
	"	\n" +
	"	vec4 ambient = calcAmbient(light.amb);\n" +
	"	vec4 diffuse = calcDiffuse(light.diff, lightDir);\n" +
	"	vec4 specular = calcSpecular(light.spec, lightDir);\n" +
	"	\n" +
	"	float attenuation = calcAttenuation(light.pos, light.attConst, light.attLin, light.attQuad);\n" +
	"	diffuse *= attenuation;\n" +
	"	specular *= attenuation;\n" +
	"	\n" +
	"	float theta = dot(lightDir, normalize(-light.dir));\n" +
	"	float epsilon = light.inner - light.outer;\n" +
	"	float intensity = clamp((theta - light.outer) / epsilon, 0.f, 1.f);\n" +
	"	\n" +
	"	diffuse *= intensity;\n" +
	"	specular *= intensity;\n" +
	"	\n" +
	"	return ambient + diffuse + specular;\n" +
	"}\n" +
	"\n" +
	"void main()\n" +
	"{\n" +
	"	fragColor = vec4(0.f);\n" +
	"	\n" +
	"	if(dirLight.en)\n" +
	"		fragColor += calcDirectionalLight();\n" +
	"	\n" +
	"	for(int i = 0; i < POINTLIGHTS; i++)\n" +
	"	{\n" +
	"		if(pointLights[i].en)\n" +
	"			fragColor += calcPointLight(i);\n" +
	"	}\n" +
	"	\n" +
	"	for(int i = 0; i < SPOTLIGHTS; i++)\n" +
	"	{\n" +
	"		if(spotLights[i].en)\n" +
	"			fragColor += calcSpotLight(i);\n" +
	"	}\n" +
	"}";
	
	var shader = new Shader(gl, vertexShaderSource, fragmentShaderSource);
	shader.maxPointLights = maxPointLights;
	shader.maxSpotLights = maxSpotLights;
	shader.camMat = new Matrix4();
	shader.norMat = new Matrix4();
	
	shader.use();
	
	shader.camPosLoc = shader.getUniform("camPos");
	shader.camMatLoc = shader.getUniform("camMat");
	shader.norMatLoc = shader.getUniform("norMat");
	shader.traMatLoc = shader.getUniform("traMat");
	
	shader.lightPointEnLoc =       new Array(maxPointLights);
	shader.lightPointPosLoc =      new Array(maxPointLights);
	shader.lightPointAmbLoc =      new Array(maxPointLights);
	shader.lightPointDiffLoc =     new Array(maxPointLights);
	shader.lightPointSpecLoc =     new Array(maxPointLights);
	shader.lightPointAttConstLoc = new Array(maxPointLights);
	shader.lightPointAttLinLoc =   new Array(maxPointLights);
	shader.lightPointAttQuadLoc =  new Array(maxPointLights);
	
	shader.lightSpotEnLoc =       new Array(maxSpotLights);
	shader.lightSpotPosLoc =      new Array(maxSpotLights);
	shader.lightSpotDirLoc =      new Array(maxSpotLights);
	shader.lightSpotAmbLoc =      new Array(maxSpotLights);
	shader.lightSpotDiffLoc =     new Array(maxSpotLights);
	shader.lightSpotSpecLoc =     new Array(maxSpotLights);
	shader.lightSpotAttConstLoc = new Array(maxSpotLights);
	shader.lightSpotAttLinLoc =   new Array(maxSpotLights);
	shader.lightSpotAttQuadLoc =  new Array(maxSpotLights);
	shader.lightSpotInnerLoc =    new Array(maxSpotLights);
	shader.lightSpotOuterLoc =    new Array(maxSpotLights);
	
	var i;
	
	shader.setUniformDirectionalLightEnable = function(en)
	{
		this.setUniform1b(this.lightDirEnLoc, en);
	};
	
	shader.lightDirEnLoc =   shader.getUniform("dirLight.en");
	shader.lightDirDirLoc =  shader.getUniform("dirLight.dir");
	shader.lightDirAmbLoc =  shader.getUniform("dirLight.amb");
	shader.lightDirDiffLoc = shader.getUniform("dirLight.diff");
	shader.lightDirSpecLoc = shader.getUniform("dirLight.spec");
	shader.setUniformDirectionalLightEnable(false);
	
	shader.setUniformPointLightEnable = function(i, en)
	{
		this.setUniform1b(this.lightPointEnLoc[i], en);
	};
	
	for(i = 0; i < maxPointLights; i++)
	{
		shader.lightPointEnLoc[i] =       shader.getUniform("pointLights[" + i + "].en");
		shader.lightPointPosLoc[i] =      shader.getUniform("pointLights[" + i + "].pos");
		shader.lightPointAmbLoc[i] =      shader.getUniform("pointLights[" + i + "].amb");
		shader.lightPointDiffLoc[i] =     shader.getUniform("pointLights[" + i + "].diff");
		shader.lightPointSpecLoc[i] =     shader.getUniform("pointLights[" + i + "].spec");
		shader.lightPointAttConstLoc[i] = shader.getUniform("pointLights[" + i + "].attConst");
		shader.lightPointAttLinLoc[i] =   shader.getUniform("pointLights[" + i + "].attLin");
		shader.lightPointAttQuadLoc[i] =  shader.getUniform("pointLights[" + i + "].attQuad");
		shader.setUniformPointLightEnable(i, false);
	}
	
	shader.setUniformSpotLightEnable = function(i, en)
	{
		this.setUniform1b(this.lightSpotEnLoc[i], en);
	};
	
	for(i = 0; i < maxSpotLights; i++)
	{
		shader.lightSpotEnLoc[i] =       shader.getUniform("spotLights[" + i + "].en");
		shader.lightSpotPosLoc[i] =      shader.getUniform("spotLights[" + i + "].pos");
		shader.lightSpotDirLoc[i] =      shader.getUniform("spotLights[" + i + "].dir");
		shader.lightSpotAmbLoc[i] =      shader.getUniform("spotLights[" + i + "].amb");
		shader.lightSpotDiffLoc[i] =     shader.getUniform("spotLights[" + i + "].diff");
		shader.lightSpotSpecLoc[i] =     shader.getUniform("spotLights[" + i + "].spec");
		shader.lightSpotAttConstLoc[i] = shader.getUniform("spotLights[" + i + "].attConst");
		shader.lightSpotAttLinLoc[i] =   shader.getUniform("spotLights[" + i + "].attLin");
		shader.lightSpotAttQuadLoc[i] =  shader.getUniform("spotLights[" + i + "].attQuad");
		shader.lightSpotInnerLoc[i] =    shader.getUniform("spotLights[" + i + "].inner");
		shader.lightSpotOuterLoc[i] =    shader.getUniform("spotLights[" + i + "].outer");
		shader.setUniformSpotLightEnable(i, false);
	}
	
	shader.matAmbColLoc =    shader.getUniform("mat.ambCol");
	shader.matAmbTexLoc =    shader.getUniform("mat.ambTex");
	shader.matAmbTexEnLoc =  shader.getUniform("mat.ambTexEn");
	shader.matDiffColLoc =   shader.getUniform("mat.diffCol");
	shader.matDiffTexLoc =   shader.getUniform("mat.diffTex");
	shader.matDiffTexEnLoc = shader.getUniform("mat.diffTexEn");
	shader.matSpecColLoc =   shader.getUniform("mat.specCol");
	shader.matSpecTexLoc =   shader.getUniform("mat.specTex");
	shader.matSpecTexEnLoc = shader.getUniform("mat.specTexEn");
	shader.matSpecShineLoc = shader.getUniform("mat.specShine");
	
	shader.render = function(model)
	{
		if(model instanceof MatMesh)
		{
			this.setUniformMaterial(model.material);
			
			if(model.material.ambientTextureEnable)
				model.material.ambientTexture.bind(0);
			if(model.material.diffuseTextureEnable)
				model.material.diffuseTexture.bind(1);
			if(model.material.specularTextureEnable)
				model.material.specularTexture.bind(2);
			
			model.mesh.bind();
			gl.drawElements(gl.TRIANGLES, model.mesh.ilength, gl.UNSIGNED_INT, 0);
		}
		else if(model instanceof Model)
		{
			this.setUniform4x4f(this.traMatLoc, model.transform);
			this.setUniform4x4f(this.norMatLoc, model.transform.calculateNormal(this.norMat));
			for(var i in model.data)
				this.render(model.data[i]);
		}
	};
	
	shader.setUniformCamera = function(camera)
	{
		this.setUniform3f(this.camPosLoc, camera.position);
		this.setUniform4x4f(this.camMatLoc, camera.calculateMatrix(this.camMat));
	};
	
	shader.setUniformMaterial = function(material)
	{
		this.setUniform4f(this.matAmbColLoc, material.ambientColor);
		this.setUniform1i(this.matAmbTexLoc, 0);
		this.setUniform1b(this.matAmbTexEnLoc, material.ambientTextureEnable);
		this.setUniform4f(this.matDiffColLoc, material.diffuseColor);
		this.setUniform1i(this.matDiffTexLoc, 1);
		this.setUniform1b(this.matDiffTexEnLoc, material.diffuseTextureEnable);
		this.setUniform4f(this.matSpecColLoc, material.specularColor);
		this.setUniform1b(this.matSpecTexLoc, 2);
		this.setUniform1b(this.matSpecTexEnLoc, material.specularTextureEnable);
		this.setUniform1f(this.matSpecShineLoc, material.specularShine);
	};
	
	shader.setUniformDirectionalLight = function(light)
	{
		this.setUniform3f(this.lightDirDirLoc,  light.direction);
		this.setUniform3f(this.lightDirAmbLoc,  light.ambient);
		this.setUniform3f(this.lightDirDiffLoc, light.diffuse);
		this.setUniform3f(this.lightDirSpecLoc, light.specular);
	};
	
	shader.setUniformPointLight = function(i, light)
	{
		this.setUniform3f(this.lightPointPosLoc[i],      light.position);
		this.setUniform3f(this.lightPointAmbLoc[i],      light.ambient);
		this.setUniform3f(this.lightPointDiffLoc[i],     light.diffuse);
		this.setUniform3f(this.lightPointSpecLoc[i],     light.specular);
		this.setUniform1f(this.lightPointAttConstLoc[i], light.attenuationConstant);
		this.setUniform1f(this.lightPointAttLinLoc[i],   light.attenuationLinear);
		this.setUniform1f(this.lightPointAttQuadLoc[i],  light.attenuationQuadratic);
	};
	
	shader.setUniformSpotLight = function(i, light)
	{
		this.setUniform3f(this.lightSpotPosLoc[i],      light.position);
		this.setUniform3f(this.lightSpotDirLoc[i],      light.direction);
		this.setUniform3f(this.lightSpotAmbLoc[i],      light.ambient);
		this.setUniform3f(this.lightSpotDiffLoc[i],     light.diffuse);
		this.setUniform3f(this.lightSpotSpecLoc[i],     light.specular);
		this.setUniform1f(this.lightSpotAttConstLoc[i], light.attenuationConstant);
		this.setUniform1f(this.lightSpotAttLinLoc[i],   light.attenuationLinear);
		this.setUniform1f(this.lightSpotAttQuadLoc[i],  light.attenuationQuadratic);
		this.setUniform1f(this.lightSpotInnerLoc[i],    light.innerCutOff);
		this.setUniform1f(this.lightSpotOuterLoc[i],    light.outerCutOff);
	};
	
	return shader;
}

function createSimpleShader(gl)
{
	var vertexShaderSource =
	"#version 300 es\n" +
	"precision highp float;\n" +
	"\n" +
	"layout (location = 0) in vec3 aPos;\n" +
	"layout (location = 2) in vec2 aTexuv;\n" +
	"\n" +
	"out vec2 texuv;\n" +
	"\n" +
	"uniform mat4 camMat, traMat;\n" +
	"\n" +
	"void main()\n" +
	"{\n" +
	"	texuv = aTexuv;\n" +
	"	gl_Position = camMat * traMat * vec4(aPos, 1.f);\n" +
	"}";
	
	var fragmentShaderSource =
	"#version 300 es\n" +
	"precision highp float;\n" +
	"\n" +
	"struct Material\n" +
	"{\n" +
	"	vec4 ambCol;\n" +
	"	sampler2D ambTex;\n" +
	"	bool ambTexEn;\n" +
	"};\n" +
	"\n" +
	"in vec2 texuv;\n" +
	"\n" +
	"out vec4 fragColor;\n" +
	"\n" +
	"uniform Material mat;\n" +
	"\n" +
	"void main()\n" +
	"{\n" +
	"	fragColor = mat.ambCol;\n" +
	"	if(mat.ambTexEn)\n" +
	"		fragColor *= texture(mat.ambTex, texuv);\n" +
	"}";
	
	var shader = new Shader(gl, vertexShaderSource, fragmentShaderSource);
	shader.camMat = new Matrix4();
	
	shader.use();
	
	shader.camMatLoc = shader.getUniform("camMat");
	shader.traMatLoc = shader.getUniform("traMat");
	
	shader.matAmbColLoc =   shader.getUniform("mat.ambCol");
	shader.matAmbTexLoc =   shader.getUniform("mat.ambTex");
	shader.matAmbTexEnLoc = shader.getUniform("mat.ambTexEn");
	
	shader.render = function(model)
	{
		if(model instanceof MatMesh)
		{
			this.setUniformMaterial(model.material);
			
			if(model.material.ambientTextureEnable)
				model.material.ambientTexture.bind(0);
			
			model.mesh.bind();
			gl.drawElements(gl.TRIANGLES, model.mesh.ilength, gl.UNSIGNED_INT, 0);
		}
		else if(model instanceof Model)
		{
			this.setUniform4x4f(this.traMatLoc, model.transform);
			for(var i in model.data)
				this.render(model.data[i]);
		}
	};
	
	shader.setUniformCamera = function(camera)
	{
		this.setUniform4x4f(this.camMatLoc, camera.calculateMatrix(this.camMat));
	};
	
	shader.setUniformMaterial = function(material)
	{
		this.setUniform4f(this.matAmbColLoc, material.ambientColor);
		this.setUniform1i(this.matAmbTexLoc, 0);
		this.setUniform1b(this.matAmbTexEnLoc, material.ambientTextureEnable);
	};
	
	return shader;
}

function updateDeltaTime()
{
	var curTime = performance.now() / 1000;
	deltaTime = curTime - __gljs_prvTime;
	__gljs_prvTime = curTime;
}

function getDeltaSpeed(originalSpeed, targetFPS = 60)
{
	return originalSpeed * deltaTime * targetFPS;
}
