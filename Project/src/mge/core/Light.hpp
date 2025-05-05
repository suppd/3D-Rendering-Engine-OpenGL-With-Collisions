#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class Light : public GameObject
{
	public:
		Light(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3( 2.0f, 10.0f, 5.0f ) );
		virtual ~Light();

        //override set parent to register/deregister light...
        virtual void _setWorldRecursively (World* pWorld) override;

        // Method to set the color of the light
        void setColor(const glm::vec3& color);

        // Method to set the ambient strength of the light
        void setAmbientStrength(float ambientStrength);

        // Method to get the color of the light
        glm::vec3 getColor() const;

        // Method to get the ambient strength of the light
        float getAmbientStrength() const;

	protected:
		glm::vec3 _color;
		float _ambientStrength;
};

#endif // LIGHT_HPP
