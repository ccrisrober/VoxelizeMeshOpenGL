#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 24) out; // 24 = 6 faces per 4 vertex/face
 
uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat3 normal;
uniform float voxelSize;

out vec3 outNormal;
out vec3 outPosition;

vec3 N;

void GenEmitVertex(in vec4 P) {
	gl_Position = modelViewProj * P;
	outPosition = (modelView * P).xyz;
	outNormal = N;
	EmitVertex();
}

void EmitQuad(vec4 p0, vec4 p1, vec4 p2, vec4 p3) {
	N = normalize(cross((p0 - p1).xyz, (p2 - p1).xyz));
  
	GenEmitVertex(p0);
	GenEmitVertex(p1);
	GenEmitVertex(p2);
	GenEmitVertex(p3);

	EndPrimitive();
}  

vec4 gen_vertex(in vec3 pos, float d1, float d2, float d3) {
	return vec4(pos + vec3(d1, d2, d3), 1.0);
}
 
void main() {
	vec3 center = (gl_in[0].gl_Position.xyz + 
						gl_in[1].gl_Position.xyz + 
						gl_in[2].gl_Position.xyz) / 3.0;
  
	float half_size = voxelSize / 2.0;
 
	// Left
	EmitQuad(
		gen_vertex(center, -half_size, -half_size, -half_size), 
		gen_vertex(center, -half_size, half_size, -half_size), 
		gen_vertex(center, -half_size, -half_size, half_size), 
		gen_vertex(center, -half_size, half_size, half_size)
	);
	// Front
	EmitQuad(
		gen_vertex(center, -half_size, -half_size, half_size), 
		gen_vertex(center, -half_size, half_size, half_size), 
		gen_vertex(center, half_size, -half_size, half_size), 
		gen_vertex(center, half_size, half_size, half_size)
	);
	// Top
	EmitQuad(
		gen_vertex(center, -half_size, half_size, half_size), 
		gen_vertex(center, -half_size, half_size, -half_size), 
		gen_vertex(center, half_size, half_size, half_size), 
		gen_vertex(center, half_size, half_size, -half_size)
	);
	// Bottom
	EmitQuad(
		gen_vertex(center, -half_size, -half_size, -half_size), 
		gen_vertex(center, -half_size, -half_size, half_size), 
		gen_vertex(center, half_size, -half_size, -half_size), 
		gen_vertex(center, half_size, -half_size, half_size)
	);
	// Right
	EmitQuad(
		gen_vertex(center, half_size, -half_size, half_size), 
		gen_vertex(center, half_size, half_size, half_size), 
		gen_vertex(center, half_size, -half_size, -half_size), 
		gen_vertex(center, half_size, half_size, -half_size)
	);
	// Back
	EmitQuad(
		gen_vertex(center, -half_size, -half_size, -half_size), 
		gen_vertex(center, -half_size, half_size, -half_size), 
		gen_vertex(center, half_size, -half_size, -half_size), 
		gen_vertex(center, half_size, half_size, -half_size)
	);
}