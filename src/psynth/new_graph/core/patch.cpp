/**
 *  Time-stamp:  <2011-06-16 20:53:21 raskolnikov>
 *
 *  @file        patch.cpp
 *  @author      Juan Pedro Bolívar Puente <raskolnikov@es.gnu.org>
 *  @date        Fri Jun 10 16:30:22 2011
 *
 *  @brief Patch implementation.
 */

/*
 *  Copyright (C) 2011 Juan Pedro Bolívar Puente
 *
 *  This file is part of Psychosynth.
 *   
 *  Psychosynth is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Psychosynth is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define PSYNTH_MODULE_NAME "psynth.graph.core.patch"

#include "new_graph/processor.hpp"
#include "new_graph/port.hpp"
#include "patch.hpp"

namespace psynth
{
namespace graph
{
namespace core
{

PSYNTH_REGISTER_NODE_STATIC (patch);

PSYNTH_DEFINE_ERROR (patch_child_error);

void patch::rt_process (rt_process_context& ctx)
{
    node::rt_process (ctx);
    for (auto& n : _rt_outputs)
        n->rt_process (ctx);
}

void patch::rt_advance ()
{
    node::rt_advance ();
    for (auto& n : _rt_childs)
        n.rt_advance ();
}
    
node_ptr patch::add (node_ptr child)
{
    if (child->is_attached_to_patch () &&
        &child->patch () == this)
        return child;
    child->check_attached_to_patch (false);
    
    child->attach_to_patch (*this);
    _childs.push_back (child);

    if (is_attached_to_process () &&
        process ().is_running ())
    {
        auto& ctx = process ().context ();
        ctx.push_rt_event (make_rt_event ([=] (rt_process_context&) {
                    this->_rt_childs.push_back (*child);
                }));
    }
    else
        _rt_childs.push_back (*child);

    return child;
}

void patch::remove (node_ptr child)
{
    if (!child->is_attached_to_patch () ||
        &child->patch () != this)
        throw patch_child_error ();

    for (auto& p : child->inputs ())
        p.disconnect ();
    for (auto& p : child->outputs ())
        p.disconnect ();

    child->detach_from_patch ();
    _childs.remove (child);

    if (is_attached_to_process () &&
        process ().is_running ())
    {
        auto& ctx = process ().context ();
        ctx.push_rt_event (make_rt_event ([=] (rt_process_context&) {
                    this->_rt_childs.remove_if (
                        base::make_equal_id (*child));
                }));
    }
    else
    {
        _rt_childs.remove_if (base::make_equal_id (*child));
    }
}

} /* namespace core */
} /* namespace graph */
} /* namespace psynth */
