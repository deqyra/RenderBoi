#include "composite_renderer.hpp"
#include "renderboi/toolbox/render/traits/render_trait.hpp"
#include "renderboi/toolbox/render/traits/render_trait_config_map.hpp"
#include "renderboi/toolbox/scene/object/component_map.hpp"
#include "traits/all_trait_renderers.hpp"

namespace renderboi
{

CompositeRenderer::CompositeRenderer() :
    _renderers(
        []<size_t ...I>(std::index_sequence<I...>) -> std::unordered_map<RenderTrait, TraitRendererPtr>
        {
            return {{
                OrderedRenderTraits[I],
                std::make_unique<typename RenderTraitMeta<OrderedRenderTraits[I]>::Renderer::type>()
            }...};
        }(std::make_index_sequence<OrderedRenderTraits.size()>{})
    )
{
}

void CompositeRenderer::render(const SceneObject& object) const
{
    const RenderTraitConfigMap& map = object.renderTraitConfigMap();

    [this, map]<size_t ...I>(std::index_sequence<I...>) -> void
    {
        (_renderers
            .at(OrderedRenderTraits[I])
            ->render(map.getConfigForTrait(OrderedRenderTraits[I])),
        ...);
    }(std::make_index_sequence<OrderedRenderTraits.size()>{});
}

}