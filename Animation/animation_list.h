#ifndef ANIMATION_LIST_H
#define ANIMATION_LIST_H

#include <queue>
#include <string>


#include "camera.h"
#include "shape.h"
#include "utils.h"


class AnimationInfo
{
public:
    int id;
    float move;
    std::string type;

    AnimationInfo(const int& in_id, const float& in_move, const std::string& in_type):
        id(in_id), move(in_move), type(in_type)
    {}
};

class AnimationBlock
{
public:
    std::vector<AnimationInfo> transforms;
    float remaining_time;

    AnimationBlock(const std::vector<AnimationInfo>& in_transforms,
                   const float& in_remaining_time):
        transforms(in_transforms), remaining_time(in_remaining_time)
        {}
};

class AnimationList
{
public:

    void add_animation(const std::vector<AnimationInfo>& animation_sequence, const float& seconds)
    {
        std::vector<AnimationInfo> transformations;
        for (auto &a: animation_sequence)
        {
            float step = a.move / seconds;
            transformations.push_back(AnimationInfo(a.id, step, a.type));
        }

        animation_queue.push(AnimationBlock(transformations, seconds));
    }

    void process_animations(std::vector<Shape*>& in_info, const float& in_delta_time)
    {
        if (animation_queue.empty())
            return;

        auto &top = animation_queue.front();
        auto &transforms = top.transforms;
        

        for (auto& t: transforms)
        {
            for (auto &shape : in_info)
            {
				if (shape->id != t.id && t.id != ALL_IDs)
					continue;
				
				auto &type = t.type;
                float move = t.move * in_delta_time;

				if (type == "MOVE_X")
					shape->traslate(Vector3(move, 0.0f, 0.0f));
				else if (type == "MOVE_Y")
					shape->traslate(Vector3(0.0f, move, 0.0f));
                else if (type == "MOVE_Z")
					shape->traslate(Vector3(0.0f, 0.0f, move));
				else if (type == "ROTATE_C_X")
                    shape->rotate_x(move);
				else if (type == "ROTATE_C_Y")
                    shape->rotate_y(move);
				else if (type == "ROTATE_C_Z")
				    shape->rotate_z(move);
				else if (type == "SCALE_X")
                    shape->scale(Vector3(1.0f + move, 1.0f, 1.0f));
                else if (type == "SCALE_Y")
                    shape->scale(Vector3(1.0f, 1.0f + move, 1.0f));
                else if (type == "SCALE_Z")
                    shape->scale(Vector3(1.0f, 1.0f, 1.0f + move));
            }
        }

        top.remaining_time -= in_delta_time;
        if (top.remaining_time <= 0)
            animation_queue.pop();
    }

    void process_animations_camera(Camera& in_camera, std::vector<Shape*>& in_info, const float& in_delta_time)
    {
        if (animation_queue.empty())
            return;

        auto &top = animation_queue.front();
        auto &transforms = top.transforms;
        

        for (auto& t: transforms)
        {
            auto &type = t.type;
            float move = t.move * in_delta_time;

            if (type == "MOVE_X")
                in_camera.traslate(Vector3(move, 0.0f, 0.0f));
            else if (type == "MOVE_Y")
                in_camera.traslate(Vector3(0.0f, move, 0.0f));
            else if (type == "MOVE_Z")
                in_camera.traslate(Vector3(0.0f, 0.0f, move));

            if (t.id == ALL_IDs)
                continue;

            for (auto &shape : in_info)
            {
				if (shape->id != t.id)
					continue;

				if (type == "ORBIT_X")
                {
                    in_camera.set_objective(shape->center);
                    in_camera.orbit_x(move);
                }
				else if (type == "ORBIT_Y")
                {
                    in_camera.set_objective(shape->center);
                    in_camera.orbit_y(move);
                }
				else if (type == "ORBIT_Z")
				{
                    in_camera.set_objective(shape->center);
                    in_camera.orbit_z(move);
                }

                break;
            }
        }

        top.remaining_time -= in_delta_time;
        if (top.remaining_time <= 0)
            animation_queue.pop();
    }

    std::queue<AnimationBlock> animation_queue;

};

#endif