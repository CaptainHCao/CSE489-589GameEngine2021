#include "CameraComponent.h"

#include "SharedTransformations.h"
#include "SkyBoxComponent.h"


std::vector<CameraComponent*> CameraComponent::activeCameras;


CameraComponent::CameraComponent( const int & cameraDepth, const float & vertFovDegrees,
								  const float & nearClip, const float & farClip )
	:Component(100), cameraDepth(cameraDepth), vertFovRadians(glm::radians(vertFovDegrees)),
	 nearClip(nearClip), farClip( farClip )
{
	componentType = CAMERA;

} // CameraComponent

CameraComponent::~CameraComponent()
{
	this->removeCamera();

}


void CameraComponent::setCameraTransformations()
{
	// Get the current width and height of the rendering window
	glm::ivec2 dimensions = owningGameObject->getOwningGame()->getWindowDimensions();

	// Clear only the portion of the window associated with the camera viewport
	// using the scissor test
	glEnable(GL_SCISSOR_TEST);

	glScissor(static_cast<GLint>(xLowerLeft * dimensions.x),
		static_cast<GLint>(yLowerLeft * dimensions.y),
		static_cast<GLint>(viewPortWidth * dimensions.x),
		static_cast<GLint>(viewPortHeight * dimensions.y));
	
	glClearColor(cameraClearColor.r, cameraClearColor.g, cameraClearColor.b, cameraClearColor.a );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_SCISSOR_TEST);

	// Set up the viewport transformation
	glViewport(static_cast<GLint>(xLowerLeft * dimensions.x),
		static_cast<GLint>(yLowerLeft * dimensions.y),
		static_cast<GLint>(viewPortWidth * dimensions.x),
		static_cast<GLint>(viewPortHeight * dimensions.y));

	// Set the projection transformation
	GLfloat aspect = static_cast<float>(viewPortWidth * dimensions.x) / (dimensions.y * viewPortHeight);
	mat4 projMat = glm::perspective(vertFovRadians, aspect, nearClip, farClip);
	SharedTransformations::setProjectionMatrix(projMat);

	// Set the viewing transformation
	SharedTransformations::setViewMatrix(glm::inverse(this->owningGameObject->getModelingTransformation()));

} // end setCameraTransformations


/**
* Sets the rendering area for the camera. In normalized coordinate the width and height
* of the viewport are 1.0.
	*
	* @param xLowerLeft normalized x coordinate of the lower left hand corner or the viewport
	* @param yLowerLeft normalized y coordinate of the lower left hand corner or the viewport
	* @param viewPortWidth normalized width of the view port in pixels
	* @param viewPortHeight normalized height of the view port in pixels
	*/
void CameraComponent::setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight)
{
	this->xLowerLeft = xLowerLeft;
	this->yLowerLeft = yLowerLeft;
	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = viewPortHeight;

} // end setViewPort


void CameraComponent::addCamera()
{
	activeCameras.push_back(this);
	std::sort(activeCameras.begin(), activeCameras.end(), CompareCameraDepth);

} // end addCamera


void CameraComponent::removeCamera()
{
	auto iter = std::find(activeCameras.begin(), activeCameras.end(), this);

	if (iter != activeCameras.end()) {

		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, activeCameras.end() - 1);
		activeCameras.pop_back();
	}

} // end removeCamera


const std::vector<CameraComponent*> CameraComponent::GetActiveCameras() 
{ 
	return activeCameras; 

} // end getActiveCameras


void CameraComponent::setCameraClearColor(vec4 clearColor) 
{ 
	cameraClearColor = clearColor; 

} // end setCameraClearColor
