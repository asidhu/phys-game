#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#define GLEW_STATIC
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include "opengl\stb_image.h"
#include "PhysGame\Source\RenderList.h"
#define rad2deg(x) (x*57.2957795131f) 
primitives renderer;

static void error_callback(int error, const char* description)
{
	//fputs(description, stderr);
}
GLFWGraphics::GLFWGraphics(int w, int h){
	m_initialized = false;
	//setup error callback
	glfwSetErrorCallback(error_callback);
	//initiate GLFW
	if (!glfwInit()){
		return;
	}
	//create a window
	m_window = glfwCreateWindow(w, h, "GLFW", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	GLenum err = GlewInit();
	if (err != GLEW_OK){
		return;
	}
	renderer.init();
	m_initialized = true;
	m_win_width = w;
	m_win_height = h;
}
void GLFWGraphics::start(){
	float  ratio;
	int width, height;
	float viewHeight = m_top - m_bottom;
	glfwGetFramebufferSize(m_window, &width, &height);
	glfwGetWindowSize(m_window, &m_win_width, &m_win_height);

	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);


	renderer.setupViewport(m_left, m_right, m_top, m_bottom );

	m_left = m_centerX - viewHeight / 2 * ratio;
	m_right = m_centerX + viewHeight / 2 * ratio;
	

}

void GLFWGraphics::loadImage(unsigned int resID, char* filename){
	int x, y, comp;
	GLuint texID;
	unsigned char* data = stbi_load(filename, &x, &y, &comp, 0);
	glGenTextures(1, &texID); 
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, x, y, 0, (comp == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
	m_resourceMap[resID] = texID;

}
void GLFWGraphics::close(){
	glfwSwapBuffers(m_window);
}
void GLFWGraphics::renderBatchTextureSquare(RenderList* list){
	float location[MAX_BATCH * 2];
	float scaling[MAX_BATCH * 2];
	float rotation[MAX_BATCH];
	GLint textures[MAX_BATCH];
	GLint texIDs[MAX_TEX];
	float texScale[MAX_BATCH * 2];
	int num = 0, numTex = 0;
	while(list->batchTexSquare.size()){
		RenderItem* item = list->batchTexSquare.top();
		list->batchTexSquare.pop();
		location[2 * num] = item->x;
		location[2 * num + 1] = item->y;
		scaling[2 * num] = item->tex.w;
		scaling[2 * num + 1] = item->tex.h;
		rotation[num] = -item->rot;
		texScale[2 * num] = 1.5f;
		texScale[2 * num + 1] = 1.5f;
		GLint texid = m_resourceMap[item->tex.resID];
		if (numTex == 0 || texIDs[numTex - 1] != texid){
			texIDs[numTex] =  texid;
			textures[num] = numTex;
			numTex++;
		}
		else{
			textures[num] = numTex-1;
		}
		num++;
		if (num == MAX_BATCH || numTex == MAX_TEX){
			renderer.batchSquareTexture(num, texIDs, numTex, textures, location, scaling, rotation, texScale);
			num = numTex = 0;
		}
	}
	for(int i=0;i<10;i++)textures[i] = 0;
	if (num != 0)
		renderer.batchSquareTexture(num, texIDs, numTex, textures, location, scaling, rotation, texScale);

}
void GLFWGraphics::renderBatchCircle(RenderList* list){
	float location[MAX_BATCH * 2];
	float radius[MAX_BATCH];
	float color[MAX_BATCH * 4];
	int num = 0;
	for (std::list<RenderItem*>::iterator it = list->batchFillCircle.begin(); it != list->batchFillCircle.end(); ++it){
		RenderItem* item = *it;
		location[2 * num] = item->x;
		location[2 * num + 1] = item->y;
		radius[num] = item->circle.radius;
		color[4 * num] = item->circle.r;
		color[4 * num + 1] = item->circle.g;
		color[4 * num + 2] = item->circle.b;
		color[4 * num + 3] = item->circle.a;
		num++;
		if (num == MAX_BATCH){
			renderer.batchCircle(num, location, radius, color);
			num = 0;
		}
	}
	if (num != 0)
		renderer.batchCircle(num, location, radius, color);

}
void GLFWGraphics::renderBatchSquare(RenderList* list){
	float location[MAX_BATCH * 2];
	float scaling[MAX_BATCH*2];
	float rotation[MAX_BATCH];
	float color[MAX_BATCH * 4];
	int num = 0;
	for (std::list<RenderItem*>::iterator it = list->batchFillSquare.begin(); it != list->batchFillSquare.end(); ++it){
		RenderItem* item = *it;
		location[2 * num] = item->x;
		location[2 * num + 1] = item->y;
		scaling[2 * num] = item->square.w;
		scaling[2 * num + 1] = item->square.h;
		rotation[num] = item->rot;
		color[4 * num] = item->circle.r;
		color[4 * num + 1] = item->circle.g;
		color[4 * num + 2] = item->circle.b;
		color[4 * num + 3] = item->circle.a;
		num++;
		if (num == MAX_BATCH){
			renderer.batchSquare(num, location, scaling, rotation, color);
			num = 0;
		}
	}
	if (num != 0)
		renderer.batchSquare(num, location, scaling, rotation, color);

}
void GLFWGraphics::drawList(RenderList* list){
	renderBatchCircle(list);
	renderBatchSquare(list);
	renderBatchTextureSquare(list);
	for (std::list<RenderItem*>::iterator it = list->renderItems.begin(); it != list->renderItems.end(); ++it){
		RenderItem* item = *it;
		if (item->myType == solidsquare || item->myType == hollowsquare){
			renderer.setColor(item->square.r, item->square.g, item->square.b, item->square.a);
			if (item->myType == hollowsquare){
				renderer.setLineWidth(item->square.lw);
				renderer.drawRect(item->x, item->y, item->square.w, item->square.h, rad2deg(item->rot));
			}
			else
				renderer.fillRect(item->x, item->y, item->square.w, item->square.h, rad2deg(item->rot));
		}
		else if (item->myType == solidcircle || item->myType == hollowcircle){
			renderer.setColor(item->circle.r, item->circle.g, item->circle.b, item->circle.a);
			if (item->myType == hollowcircle){
				renderer.setLineWidth(item->circle.lw);
				renderer.drawCircle(item->x, item->y, item->circle.radius);
			}
			else
				renderer.fillCircle(item->x, item->y, item->circle.radius);
		}
		else if (item->myType == texture){
			GLuint texid = m_resourceMap[item->tex.resID];
			const float texcoords[8] = {
				item->tex.s1 + 100, item->tex.t1 + 100,
				item->tex.s1 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t1 + 100
			};
			renderer.drawTexture(texid, item->x, item->y,texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
			//renderer.drawTexture(texid, item->x, item->y, texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
		}
		else if (item->myType == stenciltexture){
			GLuint texid = m_resourceMap[item->tex.resID];
			float texcoords[8] = {
				item->tex.s1 + 100, item->tex.t1 + 100,
				item->tex.s1 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t2 + 100,
				item->tex.s2 + 100, item->tex.t1 + 100
			};
			draw_square mask, tex;
			tex.x = tex.y = 0;
			tex.w = tex.h = 100;
			mask.x = item->x;
			mask.y = item->y;
			mask.w = item->tex.w;
			mask.h = item->tex.h;
			renderer.drawStenciledTexture(texid, mask, tex, texcoords, rad2deg(item->rot));
		}
	}
}