#ifndef ANIMATION_LIST_H
#define ANIMATION_LIST_H

#include <queue>
#include <string>

#include "draw_info.h"

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
    int remaining_frames;

    AnimationBlock(const std::vector<AnimationInfo>& in_transforms,
                   const int& in_remaining_frames):
        transforms(in_transforms), remaining_frames(in_remaining_frames)
        {}
};

class AnimationList
{
public:

    void add_animation(int in_id, const std::vector<AnimationInfo>& animation_sequence, int frames)
    {
        std::vector<AnimationInfo> transformations;
        for (auto &a: animation_sequence)
        {
            float step = a.move/float(frames);
            transformations.push_back(AnimationInfo(in_id, step, a.type));
        }

        animation_queue.push(AnimationBlock(transformations, frames));
    }

    void process_animations(std::vector<DrawInfo>& in_info)
    {
        if (animation_queue.empty())
            return;

        auto &top = animation_queue.front();
        auto &transforms = top.transforms;
        

        for (auto& t: transforms)
        {
            for (auto &i : in_info)
            {
                if (i.id == t.id)
                {
                    auto &type = t.type;

                    if (type == "MOVE_X")
                        i.model.traslate(t.move, 0.0f, 0.0f);
                    else if (type == "MOVE_Y")
                        i.model.traslate(0.0f, t.move, 0.0f);
                    else if (type == "ROTATE_X")
                        i.model.rotate_x(t.move);
                    else if (type == "ROTATE_Y")
                        i.model.rotate_y(t.move);
                    else if (type == "ROTATE_Z")
                        i.model.rotate_z(t.move);
                    else if (type == "ROTATE_C_X")
                    {
                        i.model.traslate(-i.c_x, -i.c_y, -i.c_z);
                        i.model.rotate_x(t.move);
                        i.model.traslate(i.c_x, i.c_y, i.c_z);
                    }
                    else if (type == "ROTATE_C_Y")
                    {
                        i.model.traslate(-i.c_x, -i.c_y, -i.c_z);
                        i.model.rotate_y(t.move);
                        i.model.traslate(i.c_x, i.c_y, i.c_z);
                    }
                    else if (type == "ROTATE_C_Z")
                    {
                        i.model.traslate(-i.c_x, -i.c_y, -i.c_z);
                        i.model.rotate_z(t.move);
                        i.model.traslate(i.c_x, i.c_y, i.c_z);
                    }
                    else if (type == "SCALE")
                        i.model.scale(t.move, t.move, t.move);
                }
            }
        }

        top.remaining_frames--;
        if (top.remaining_frames <= 0)
            animation_queue.pop();
    }

    std::queue<AnimationBlock> animation_queue;

};

#endif