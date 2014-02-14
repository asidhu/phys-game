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
	GLenum err = glewInit();
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_left, m_right, m_bottom, m_top, 1.f, -1.f);
	m_left = m_centerX - viewHeight / 2 * ratio;
	m_right = m_centerX + viewHeight / 2 * ratio;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLFWGraphics::loadImage(unsigned int resID, char* filename){
	int x, y, comp;
	GLuint texID;
	unsigned char* data = stbi_load(filename, &x, &y, &comp, 0);
	glGenTextures(1, &texID); 
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, (comp == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, datapix);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_resourceMap[resID] = texID;

}
void GLFWGraphics::close(){
	glfwSwapBuffers(m_window);
}
void GLFWGraphics::drawList(RenderList* list){
	for (std::list<RenderItem*>::iterator it = list->renderItems.begin(); it != list->renderItems.end(); it++){
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
				item->tex.s1, item->tex.t1,
				item->tex.s1, item->tex.t2,
				item->tex.s2, item->tex.t2,
				item->tex.s2, item->tex.t1
			};
			renderer.drawTexture(texid, item->x, item->y,texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
			//renderer.drawTexture(texid, item->x, item->y, texcoords, item->tex.w, item->tex.h, rad2deg(item->rot));
		}
		else if (item->myType == stenciltexture){
			GLuint texid = m_resourceMap[item->tex.resID];
			float texcoords[8] = {
				item->tex.s1, item->tex.t1,
				item->tex.s1, item->tex.t2,
				item->tex.s2, item->tex.t2,
				item->tex.s2, item->tex.t1
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