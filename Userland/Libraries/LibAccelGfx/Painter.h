/*
 * Copyright (c) 2023, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Noncopyable.h>
#include <AK/Vector.h>
#include <LibAccelGfx/Canvas.h>
#include <LibAccelGfx/Forward.h>
#include <LibAccelGfx/Program.h>
#include <LibGfx/AffineTransform.h>
#include <LibGfx/Forward.h>

namespace AccelGfx {

class Painter {
    AK_MAKE_NONCOPYABLE(Painter);
    AK_MAKE_NONMOVABLE(Painter);

public:
    static OwnPtr<Painter> create();

    Painter(Context&);
    ~Painter();

    void clear(Gfx::Color);

    void save();
    void restore();

    [[nodiscard]] Gfx::AffineTransform const& transform() const { return state().transform; }
    void set_transform(Gfx::AffineTransform const& transform) { state().transform = transform; }

    void fill_rect(Gfx::FloatRect, Gfx::Color);
    void fill_rect(Gfx::IntRect, Gfx::Color);

    enum class ScalingMode {
        NearestNeighbor,
        Bilinear,
    };

    void draw_scaled_bitmap(Gfx::FloatRect const& dst_rect, Gfx::Bitmap const&, Gfx::FloatRect const& src_rect, ScalingMode = ScalingMode::NearestNeighbor);
    void draw_scaled_bitmap(Gfx::IntRect const& dst_rect, Gfx::Bitmap const&, Gfx::IntRect const& src_rect, ScalingMode = ScalingMode::NearestNeighbor);

    void set_canvas(Canvas& canvas) { m_canvas = canvas; }
    void flush();

private:
    Context& m_context;
    Optional<Canvas> m_canvas;

    struct State {
        Gfx::AffineTransform transform;
    };

    [[nodiscard]] State& state() { return m_state_stack.last(); }
    [[nodiscard]] State const& state() const { return m_state_stack.last(); }

    [[nodiscard]] Gfx::FloatRect to_clip_space(Gfx::FloatRect const& screen_rect) const;

    Vector<State, 1> m_state_stack;

    Program m_rectangle_program;
    Program m_blit_program;
};

}
