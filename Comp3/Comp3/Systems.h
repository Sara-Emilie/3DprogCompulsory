#include "Components.h"
#include "glm/glm.hpp"
#include "ComponentManager.h"
#include "Mesh.h"

struct Entity 
{
	int ID;
};


class MovementSystem
{
    private:
    ComponentManager<PositionComponent>* posComp;
    ComponentManager<VelocityComponent>* velComp;

public:
    MovementSystem(ComponentManager<PositionComponent>* positions, ComponentManager<VelocityComponent>* velocity)
    {
        this->posComp = positions;
        this->velComp = velocity;
    }

    void Move(std::vector<Entity> entity, float deltaTime) 
    {
        for (Entity enti : entity)
        {
           
            if (posComp->HasComponent(enti.ID) && velComp->HasComponent(enti.ID))
            {
               
                PositionComponent pos = posComp->GetComponent(enti.ID);
                VelocityComponent vel = velComp->GetComponent(enti.ID);

              
                pos.x[enti.ID] += vel.vx[enti.ID] * deltaTime;
                pos.y[enti.ID] += vel.vy[enti.ID] * deltaTime;
                pos.z[enti.ID] += vel.vz[enti.ID] * deltaTime;

                posComp->AddComponent(enti.ID, pos);
                velComp->AddComponent(enti.ID, vel);
            }
        }
    }
};


class RenderingSystem 
{
public:
    
    ComponentManager<PositionComponent>* posComp;
    ComponentManager<MeshComponent>* meshComp;

    RenderingSystem(ComponentManager<PositionComponent>* positions, ComponentManager<MeshComponent>* mesh)
    {
        this->posComp = positions;
        this->meshComp = mesh;
    }

    void Render(std::vector<Entity> entity, ShaderClass& shaderProgram) 
    {
        if (entity.empty()) {
            std::cerr << "No entities to render" << std::endl;
            return;
        }

        shaderProgram.Activate();
        for (Entity enti : entity) 
        {
            if (posComp->HasComponent(enti.ID) && meshComp->HasComponent(enti.ID))
            {
                meshComp->GetComponent(enti.ID).meshes[enti.ID].VAO.Bind();
                PositionComponent pos = posComp->GetComponent(enti.ID);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(pos.x[enti.ID], pos.y[enti.ID], pos.z[enti.ID]));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                meshComp->GetComponent(enti.ID).meshes[enti.ID].Draw(shaderProgram);
            }
        }
    }
};


class CollisionSystem 
{
//    ComponentManager<PositionComponent>* posComp;
//    ComponentManager<VelocityComponent>* velComp;
//    ComponentManager<RadiusComponent>* radiusComp;
//
//public:
//    CollisionSystem(ComponentManager<PositionComponent>* positions,
//        ComponentManager<VelocityComponent>* velocities,
//        ComponentManager<RadiusComponent>* radii)
//        : posComp(positions), velComp(velocities), radiusComp(radii) {}
//
//
//    void CollideWithBall(PositionComponent& positions, VelocityComponent& velocities, RadiusComponent& radii, size_t entity1, size_t entity2)
//    {
//        glm::vec3 Positionball(positions.x[entity1], positions.y[entity1], positions.z[entity1]);
//        glm::vec3 Positionotherball(positions.x[entity2], positions.y[entity2], positions.z[entity2]);
//
//        glm::vec3 Extentball(radii.radii[entity1], radii.radii[entity1], radii.radii[entity1]);
//        glm::vec3 Extentotherball(radii.radii[entity2], radii.radii[entity2], radii.radii[entity2]);
//
//        glm::vec3 velocity = glm::vec3(velocities.vx[entity1], velocities.vy[entity1], velocities.vz[entity1]);
//
//        glm::vec3 min = Positionotherball - Extentotherball;
//        glm::vec3 max = Positionotherball + Extentotherball;
//        glm::vec3 spheremin = Positionball;
//        glm::vec3 spheremax = Positionotherball;
//
//        // Find the closest point to the other object
//        glm::vec3 closestpoint = glm::clamp(spheremin, min, max);
//
//        float diameter = glm::distance(spheremin, spheremax);
//        glm::vec3 distance = spheremin - spheremax;
//
//        if (diameter <= (Extentball.x + Extentotherball.x) && diameter > 0) {
//            // Reflect velocity
//            velocity = glm::reflect(velocity, glm::normalize(distance));
//            velocities.vx[entity1] = velocity.x;
//            velocities.vy[entity1] = velocity.y;
//            velocities.vz[entity1] = velocity.z;
//        }
//    }

};
