// MIT License

// Copyright (c) 2025 Mateusz Gancarz

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "InverseKinematics/Segment.h"

namespace chs::ik
{
    Segment::Segment(const glm::mat4& world_transform)
        : world_transform{world_transform}, local_transform{world_transform} {}

    void Segment::addChild(std::unique_ptr<Segment> child)
    {
        child_segment = std::move(child);
        child_segment->parent_segment = this;
        child_segment->setWorldTransform(child_segment->worldTransform());
    }

    void Segment::setWorldTransform(const glm::mat4& transform)
    {
        world_transform = transform;
        local_transform = glm::inverse(getParentWorldTransform()) * world_transform;

        if (child_segment)
        {
            const glm::mat4 child_new_world_transform = world_transform * child_segment->localTransform();
            child_segment->setWorldTransform(child_new_world_transform);
        }
    }

    glm::mat4 Segment::getParentWorldTransform() const
    {
        return parent_segment ? parent_segment->worldTransform() : glm::mat4{1.0f};
    }

    void Segment::setLocalTransform(const glm::mat4& transform)
    {
        local_transform = transform;
        world_transform = getParentWorldTransform() * local_transform;

        if (child_segment)
        {
            const glm::mat4 child_new_world_transform = world_transform * child_segment->localTransform();
            child_segment->setWorldTransform(child_new_world_transform);
        }
    }

    void Segment::forceWorldTransform(const glm::mat4& transform)
    {
        world_transform = transform;
    }

    void Segment::Segment::refresh()
    {
        local_transform = glm::inverse(getParentWorldTransform()) * world_transform;

        if (child_segment)
        {
            child_segment->refresh();
        }
    }

    Segment& Segment::parent()
    {
        return parentImpl();
    }

    Segment& Segment::parentImpl() const
    {
        assert(hasParent() && "Cannot call 'parent' while the parent is null!");
        return *parent_segment;
    }

    const Segment& Segment::parent() const
    {
        return parentImpl();
    }

    Segment& Segment::child()
    {
        return childImpl();
    }

    Segment& Segment::childImpl() const
    {
        assert(hasChild() && "Cannot call 'child' while the child is null!");
        return *child_segment;
    }

    const Segment& Segment::child() const
    {
        return childImpl();
    }

    glm::vec3 Segment::worldOrigin() const
    {
        if (hasParent())
        {
            return parent_segment->worldEnd();
        }

        return glm::vec3{0.0f};
    }

    glm::vec3 Segment::worldEnd() const
    {
        return world_transform * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
    }

    float Segment::length() const
    {
        static constexpr int TRANSLATION_COLUMN_INDEX = 3;
        return glm::length(glm::vec3{local_transform[TRANSLATION_COLUMN_INDEX]});
    }
}
