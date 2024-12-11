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
    ComponentManager<PositionComponent>* posComp;
    ComponentManager<VelocityComponent>* velComp;
    ComponentManager<RadiusComponent>* radComp;

public:
    CollisionSystem(ComponentManager<PositionComponent>* positions,ComponentManager<VelocityComponent>* velocities, ComponentManager<RadiusComponent>* radii)
        : posComp(positions), velComp(velocities), radComp(radii) {}


    void CollideWithBall(std::vector<Entity> entities)
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {
            for (size_t j = i + 1; j < entities.size(); ++j)
            {
                Entity entity1 = entities[i];
                Entity entity2 = entities[j];

                // Check if both entities have the required components
                if (posComp->HasComponent(entity1.ID) && velComp->HasComponent(entity1.ID) && radComp->HasComponent(entity1.ID) &&
                    posComp->HasComponent(entity2.ID) && velComp->HasComponent(entity2.ID) && radComp->HasComponent(entity2.ID))
                {
                    // Get the position, radius, and velocity components for both entities
                    PositionComponent pos1 = posComp->GetComponent(entity1.ID);
                    PositionComponent pos2 = posComp->GetComponent(entity2.ID);

                    VelocityComponent vel1 = velComp->GetComponent(entity1.ID);
                    VelocityComponent vel2 = velComp->GetComponent(entity2.ID);

                    RadiusComponent rad1 = radComp->GetComponent(entity1.ID);
                    RadiusComponent rad2 = radComp->GetComponent(entity2.ID);

                    // Convert positions to glm::vec3
                    glm::vec3 pos1Vec(pos1.x[entity1.ID], pos1.y[entity1.ID], pos1.z[entity1.ID]);
                    glm::vec3 pos2Vec(pos2.x[entity2.ID], pos2.y[entity2.ID], pos2.z[entity2.ID]);

                    // Calculate the distance between the centers of the two entities
                    float distance = glm::distance(pos1Vec, pos2Vec);

                  
                    if (distance < (1 + 1)) //TODO FIX (hardcoded radius for now)
                    {
                        std::cout << "hit" << std::endl;
                        // Create glm::vec3 velocity vectors from the individual components for each entity
                        glm::vec3 vel1Vec(vel1.vx[entity1.ID], vel1.vy[entity1.ID], vel1.vz[entity1.ID]);
                        glm::vec3 vel2Vec(vel2.vx[entity2.ID], vel2.vy[entity2.ID], vel2.vz[entity2.ID]);

                        //Calculate the relative velocity vector between the two entities
                        glm::vec3 relativeVelocity = vel1Vec - vel2Vec;

                        // Reflect the velocities of the two entities
                        glm::vec3 normal = glm::normalize(pos2Vec - pos1Vec); // Normal of collision surface

                        //elastic collision
                        float velocity1Dot = glm::dot(relativeVelocity, normal);
                        float velocity2Dot = glm::dot(relativeVelocity, normal);

                        glm::vec3 velocity1 = vel1Vec - (2.0f * velocity1Dot * normal);
                        glm::vec3 velocity2 = vel2Vec - (2.0f * velocity2Dot * normal);

                        //Update the velocities for the entities
                        vel1.vx[entity1.ID] = velocity1.x;
                        vel1.vy[entity1.ID] = velocity1.y;
                        vel1.vz[entity1.ID] = velocity1.z;

                        vel2.vx[entity2.ID] = -velocity2.x;
                        vel2.vy[entity2.ID] = -velocity2.y;
                        vel2.vz[entity2.ID] = -velocity2.z;

            
                        velComp->AddComponent(entity1.ID, vel1);
                        velComp->AddComponent(entity2.ID, vel2);

                    }
                }
            }
        }
    }

};
