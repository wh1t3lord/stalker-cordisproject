#include "StdAfx.h"

#include "Animation.h"

const channal_rule channels::rules[channels::max] = {{mix_type::eAnimationLerp, mix_type::eAnimationLerp},
    {mix_type::eAnimationLerp, mix_type::eAnimationLerp}, {mix_type::eAnimationLerp, mix_type::eAnimationAdd},
    {mix_type::eAnimationLerp, mix_type::eAnimationAdd}};

channels::channels() { init(); }
void channels::init() { std::fill(factors, factors + max, 1.f); }
void channels::set_factor(u16 channel, float factor)
{
    VERIFY(channel < max);
    factors[channel] = factor;
}
