//glm
#include "glm\ext.hpp"

//naga
#include "camera_component.hpp"
#include "platform.hpp"
#include "gpu.hpp"
#include "collision.hpp"
#include "game_object.hpp"

namespace naga
{
	const char* CameraComponent::component_name = "CameraComponent";

	CameraParameters CameraComponent::get_parameters(const GpuViewportType& viewport) const
    {
		const auto aspect = glm::max(glm::epsilon<f32>(), static_cast<f32>(viewport.width) / glm::max(static_cast<f32>(viewport.height), 1.0f));
        const auto& pose = get_owner()->pose;

		CameraParameters parameters;
		parameters.location = pose.location;

        switch (projection_type)
        {
		case ProjectionType::ORTHOGRAPHIC:
			parameters.projection_matrix = glm::ortho(viewport.x, viewport.x + viewport.width, viewport.y + viewport.height, viewport.y);
            break;
		case ProjectionType::PERSPECTIVE:
			parameters.projection_matrix = glm::perspective(fov, aspect, near, far);
            break;
        }

        const auto forward = vec3(0, 0, 1) * glm::mat3_cast(pose.rotation);
        const auto left = glm::normalize(glm::cross(vec3(0, 1, 0), forward));	// TODO: this can cause a crash if they are parallel!!
        auto up = glm::normalize(glm::cross(forward, left));

        // rotate up matrix along forward axis
        up = glm::rotate(glm::angleAxis(roll, forward), up);

		parameters.frustum.set(pose.location, left, up, forward, fov, near, far, aspect);
		parameters.view_matrix = glm::lookAt(pose.location, pose.location + forward, up);

        return parameters;
    }
    
    // TODO: there is an inherent problem here in that get_ray will likely be called before
    // the parameters have been calculated properly, and will cause the results of this
    // function to lag one frame behind what's actually on the screen.
    // 
    // one solution would be to have components know when their pose (eg. on_pose_changed)
    // has been changed. in this case, we would listen for on_pose_changed, and then, if
    // necessary, we would recalculate the parameters (by calling get_params)
    // and then get the ray as normal. this would be problematic because it would 
    // probably add thousands of calls to components that have no business responding
    // to pose changes.
    //
    // the other solution would be to always recalculate the matrices every frame.
    // the calculations are negligible and would only be happening once per frame.
    //
    // another option is to simply have @get_ray accept a viewport, and calculate the 
    // matrices before we use them in the unProject functions. this would probably be
    // perfectly acceptable since get_ray wouldn't likely be happening more than
    // once per frame.
	Line3 CameraComponent::get_ray(const GpuViewportType& viewport, const vec2& screen_location) const
	{
		// TODO: this is problematic because the viewport isn't necessarily going to be stacked on top, maybe we need to pass in the viewport here?
		Line3 ray;
		auto parameters = get_parameters(viewport);
		ray.start = glm::unProject(vec3(screen_location, 0), parameters.view_matrix, parameters.projection_matrix, vec4(viewport.x, viewport.y, viewport.width, viewport.height));
		ray.end = glm::unProject(vec3(screen_location, 1), parameters.view_matrix, parameters.projection_matrix, vec4(viewport.x, viewport.y, viewport.width, viewport.height));
		return ray;
	}
}
