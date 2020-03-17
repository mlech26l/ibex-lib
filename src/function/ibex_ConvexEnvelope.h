#ifndef __IBEX_CONVEX_ENVELOPE_H__
#define __IBEX_CONVEX_ENVELOPE_H__

#include <vector>
#include "ibex_CmpOp.h"
#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "ibex_Fnc.h"

namespace ibex {

class ConvexEnvelope {
public:

    struct LinearConstraint {
        double y;
        std::vector<double> x;
        double rhs;
        CmpOp op;
    };

    ConvexEnvelope(int nb_var);

    int nb_var;

    void add_linear_constraint(const LinearConstraint& lc);
    int nb_envelope() const;
    void clear();
    void merge_envelope(const ConvexEnvelope& ce);
    LinearConstraint& operator[](int index);
    const LinearConstraint& operator[](int index) const;

private:
    std::vector<LinearConstraint> linear_constraints_;
};

inline std::ostream& operator<<(std::ostream& os, const ConvexEnvelope::LinearConstraint& lc) {
    os << "LC(" << lc.y << "y";
    for(int i = 0; i < lc.x.size(); ++i) {
        os << " + " << lc.x[i] << " x" << i;
    }
    if(lc.op == CmpOp::LEQ) {
        os << " <= ";
    } else if(lc.op == CmpOp::GEQ) {
        os << " >= ";
    } else {
        os << " == ";
    }
    os << lc.rhs << ")";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ConvexEnvelope& ce) {
    for(int i = 0; i < ce.nb_envelope(); ++i) {
        os << ce[i] << std::endl;
    }
    return os;
}

namespace convex_envelope {
#define DECLARE_MAX_LIN_CONST(name) extern const int MAX_LIN_##name;

    DECLARE_MAX_LIN_CONST(add)
    ConvexEnvelope add(const Interval& x1, const Interval& x2);
    DECLARE_MAX_LIN_CONST(sub)
    ConvexEnvelope sub(const Interval& x1, const Interval& x2);
    DECLARE_MAX_LIN_CONST(mul)
    ConvexEnvelope mul(const Interval& x1, const Interval& x2);
    DECLARE_MAX_LIN_CONST(int_mul)
    ConvexEnvelope int_mul(int a, const Interval& x);
    DECLARE_MAX_LIN_CONST(sqr)
    ConvexEnvelope sqr(const Interval& x, int count = 3);
    DECLARE_MAX_LIN_CONST(sqrt)
    ConvexEnvelope sqrt(const Interval& x, int count = 3);
    DECLARE_MAX_LIN_CONST(exp)
    ConvexEnvelope exp(const Interval& x, int count = 3);
    DECLARE_MAX_LIN_CONST(log)
    ConvexEnvelope log(const Interval& x, int count = 3);
    DECLARE_MAX_LIN_CONST(cos)
    ConvexEnvelope cos(const Interval& x);
    DECLARE_MAX_LIN_CONST(sin)
    ConvexEnvelope sin(const Interval& x);
    DECLARE_MAX_LIN_CONST(tan)
    ConvexEnvelope tan(const Interval& x);
    DECLARE_MAX_LIN_CONST(cosh)
    ConvexEnvelope cosh(const Interval& x, int count = 3);
    DECLARE_MAX_LIN_CONST(acosh)
    ConvexEnvelope acosh(const Interval& x);
    DECLARE_MAX_LIN_CONST(sinh)
    ConvexEnvelope sinh(const Interval& x);
    DECLARE_MAX_LIN_CONST(asinh)
    ConvexEnvelope asinh(const Interval& x);
    DECLARE_MAX_LIN_CONST(tanh)
    ConvexEnvelope tanh(const Interval& x);
    DECLARE_MAX_LIN_CONST(atanh)
    ConvexEnvelope atanh(const Interval& x);
    DECLARE_MAX_LIN_CONST(acos)
    ConvexEnvelope acos(const Interval& x);
    DECLARE_MAX_LIN_CONST(asin)
    ConvexEnvelope asin(const Interval& x);
    DECLARE_MAX_LIN_CONST(atan)
    ConvexEnvelope atan(const Interval& x);
    DECLARE_MAX_LIN_CONST(floor)
    ConvexEnvelope floor(const Interval& x);
    DECLARE_MAX_LIN_CONST(ceil)
    ConvexEnvelope ceil(const Interval& x);
    DECLARE_MAX_LIN_CONST(saw)
    ConvexEnvelope saw(const Interval& x);
    DECLARE_MAX_LIN_CONST(abs)
    ConvexEnvelope abs(const Interval& x);
    DECLARE_MAX_LIN_CONST(sign)
    ConvexEnvelope sign(const Interval& x);
    DECLARE_MAX_LIN_CONST(max)
    ConvexEnvelope max(const Interval& x1, const Interval& x2);
    DECLARE_MAX_LIN_CONST(min)
    ConvexEnvelope min(const Interval& x1, const Interval& x2);
    DECLARE_MAX_LIN_CONST(div_from_mul)
    ConvexEnvelope div_from_mul(const Interval& x1, const Interval& x2, const Interval& y);
    DECLARE_MAX_LIN_CONST(power)
    ConvexEnvelope power(const Interval& x, int p);

    ConvexEnvelope cos_offset(const Interval& x, const Interval& offset, bool negate);
    bool find_secante(double x_start, double search_lb, double search_ub, const Fnc& f, Interval& solution, int max_iter=100);

    enum class EnvelopeType {Over, Under};
    enum class IntervalSide {Left, Right};

    ConvexEnvelope::LinearConstraint tangente(const Interval& x, const Interval& x_value, const Interval& x_deriv, EnvelopeType env_type);
    ConvexEnvelope::LinearConstraint tangente_under(const Interval& x, const Interval& x_value, const Interval& x_deriv);
    ConvexEnvelope::LinearConstraint tangente_over(const Interval& x, const Interval& x_value, const Interval& x_deriv);
    ConvexEnvelope::LinearConstraint line(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2, EnvelopeType env_type);
    ConvexEnvelope::LinearConstraint line_under(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2);
    ConvexEnvelope::LinearConstraint line_over(const Interval& x1, const Interval& x_value1, const Interval& x2, const Interval& x_value2);
    ConvexEnvelope::LinearConstraint bound(CmpOp op, double rhs);
    ConvexEnvelope::LinearConstraint bound(CmpOp op, double rhs);

    struct ConvexConcaveProperties {
        bool infinite_lb = false;
        bool infinite_ub = false;
        bool vertical_lb_tangente = false;
        bool vertical_ub_tangente = false;
        Interval inflection_point = 0.0;
        const Fnc& secante_func;

        ConvexConcaveProperties(const Interval& inflection_point, const Fnc& secante_func)
            : inflection_point(inflection_point), secante_func(secante_func) {
        }
    };

    DECLARE_MAX_LIN_CONST(CONVEX)
    template<typename Func, typename Deriv>
    inline ConvexEnvelope convex_function(const Interval& x, const Func& func, const Deriv& deriv,
    bool vertical_lb_tangente=false, bool vertical_ub_tangente=false) {
        ConvexEnvelope ce(1);
        const Interval f_xlb = func(x.lb());
        const Interval f_xub = func(x.ub());
        const Interval g_xlb = deriv(x.lb());
        const Interval g_xub = deriv(x.ub());
        if(!vertical_lb_tangente && !vertical_ub_tangente) {
            ce.add_linear_constraint(line_over(x.lb(), f_xlb, x.ub(), f_xub));
        }
        if(vertical_lb_tangente) {
            ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
        } else {
            ce.add_linear_constraint(tangente_under(x.lb(), f_xlb, g_xlb));
        }
        if(vertical_ub_tangente) {
            ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
        } else {
            ce.add_linear_constraint(tangente_under(x.ub(), f_xub, g_xub));
        }
        if(!x.is_degenerated()) {
            ce.add_linear_constraint(tangente_under(x.mid(), func(x.mid()), deriv(x.mid())));
        }
        return ce;
    }

    DECLARE_MAX_LIN_CONST(CONCAVE)
    template<typename Func, typename Deriv>
    inline ConvexEnvelope concave_function(const Interval& x, const Func& func, const Deriv& deriv,
    bool vertical_lb_tangente=false, bool vertical_ub_tangente=false) {
        ConvexEnvelope ce(1);
        const Interval f_xlb = func(x.lb());
        const Interval f_xub = func(x.ub());
        const Interval g_xlb = deriv(x.lb());
        const Interval g_xub = deriv(x.ub());
        if(!vertical_lb_tangente && !vertical_ub_tangente) {
            ce.add_linear_constraint(line_under(x.lb(), f_xlb, x.ub(), f_xub));
        }
        if(vertical_lb_tangente) {
            ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
        } else {
            ce.add_linear_constraint(tangente_over(x.lb(), f_xlb, g_xlb));
        }
        if(vertical_ub_tangente) {
            ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
        } else {
            ce.add_linear_constraint(tangente_over(x.ub(), f_xub, g_xub));
        }
        if(!x.is_degenerated()) {
            ce.add_linear_constraint(tangente_over(x.mid(), func(x.mid()), deriv(x.mid())));
        }
        return ce;
    }

    inline ConvexEnvelope::LinearConstraint bound(CmpOp op, double rhs) {
        if(op == CmpOp::LEQ) {
            return ConvexEnvelope::LinearConstraint{0,{1}, rhs, CmpOp::LEQ};
        } else {
            return ConvexEnvelope::LinearConstraint{0, {-1}, -rhs, CmpOp::LEQ};
        }
    }

    inline ConvexEnvelope::LinearConstraint bound_node(CmpOp op, double rhs) {
        if(op == CmpOp::LEQ) {
            return ConvexEnvelope::LinearConstraint{1,{0}, rhs, CmpOp::LEQ};
        } else {
            return ConvexEnvelope::LinearConstraint{-1, {0}, -rhs, CmpOp::LEQ};
        }
    }

    template<typename Func, typename Deriv>
    ConvexEnvelope::LinearConstraint centered_form(const Interval& x, const Func& func, const Deriv& deriv, const EnvelopeType envelope_type, const IntervalSide side, bool vertical_tangente) {
        if(vertical_tangente) {
            if(side == IntervalSide::Left) {
                return bound(CmpOp::GEQ, x.lb());
            } else {
                return bound(CmpOp::LEQ, x.ub());
            }
        }
        double x_coef;
        double rhs;
        double y_coef;
        Interval d = deriv(x);
        double my_x;
        double my_d;
        if(envelope_type == EnvelopeType::Over) {
            // Over y <= ...
            switch(side) {
            case IntervalSide::Left:
                my_d = d.ub();
                my_x = x.lb();
                break;
            case IntervalSide::Right:
                my_d = d.lb();
                my_x = x.ub();
                break;
            }
            y_coef = 1;
            x_coef = -my_d;
            rhs = (func(x).ub() - Interval(my_d)*my_x).ub();
        } else {
            // Under y >= ...
            switch(side) {
            case IntervalSide::Left:
                my_d = d.lb();
                my_x = x.lb();
                break;
            case IntervalSide::Right:
                my_d = d.ub();
                my_x = x.ub();
                break;
            }
            y_coef = -1;
            x_coef = my_d;
            rhs = -((func(x).lb() - Interval(my_d)*my_x).lb());
        }
        return ConvexEnvelope::LinearConstraint{y_coef, {x_coef}, rhs, CmpOp::LEQ};
    }

    template<typename Func, typename Deriv>
    void left_part(const Interval& x, const Func& func, const Deriv& deriv, const ConvexConcaveProperties& prop, EnvelopeType env_type, ConvexEnvelope& ce) {
        using LC = ConvexEnvelope::LinearConstraint;
        if(prop.infinite_lb) {
                ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
        } else {
            /* We need an upper bound not to large for the newton to effective.
            Upper bound -x.lb() is an heuristic derived from tanh,
            visually we see that the secante point in always less than -x.lb().
            Since the other functions have some sort of symmetries around 0,
            this seems to be a good choice: x.lb() and secante_left seem
            to always meet at 0. Maybe TODO set ub for secante search for each function.
            idem for the secante_right */
            Interval secante_left;
            bool secante_found = find_secante(x.lb(), (prop.inflection_point).ub(), (2*prop.inflection_point-x.lb()).lb(), prop.secante_func, secante_left);
            // std::cout <<"secl" << secante_left << std::endl;
            if(secante_found && secante_left.lb() >= x.ub()) {
                const LC lc = line(x.lb(), func(x.lb()), x.ub(), func(x.ub()), env_type);
                ce.add_linear_constraint(lc);
            } else if(secante_found) {
                const LC lc = tangente(secante_left, func(secante_left), deriv(secante_left), env_type);
                ce.add_linear_constraint(lc);
                if(secante_left.ub() <= x.ub()) {
                    if(prop.vertical_ub_tangente) {
                        ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
                    } else {
                        const LC lc2 = tangente(x.ub(), func(x.ub()), deriv(x.ub()), env_type);
                        ce.add_linear_constraint(lc2);
                    }
                }
                // + middle ?
            } else {
                // default to centered form
                IntervalSide side = IntervalSide::Left;
                ce.add_linear_constraint(centered_form(x, func, deriv, env_type, side, prop.vertical_lb_tangente));
            }
        }
    }

    template<typename Func, typename Deriv>
    void right_part(const Interval& x, const Func& func, const Deriv& deriv, const ConvexConcaveProperties& prop, EnvelopeType env_type, ConvexEnvelope& ce) {
        using LC = ConvexEnvelope::LinearConstraint;
        if(prop.infinite_ub) {
                ce.add_linear_constraint(bound(CmpOp::LEQ, x.ub()));
        } else {
            Interval secante_right;
            bool secante_found = find_secante(x.ub(), (2*prop.inflection_point-x.ub()).ub(), prop.inflection_point.lb(), prop.secante_func, secante_right);
            // std::cout <<"secr" << secante_right << std::endl;
            if(secante_found && secante_right.ub() <= x.lb()) {
                const LC lc = line(x.lb(), func(x.lb()), x.ub(), func(x.ub()), env_type);
                ce.add_linear_constraint(lc);
            } else if(secante_found) {
                const LC lc = tangente(secante_right, func(secante_right), deriv(secante_right), env_type);
                ce.add_linear_constraint(lc);
                if(secante_right.lb() >= x.lb()) {
                    if(prop.vertical_lb_tangente) {
                        ce.add_linear_constraint(bound(CmpOp::GEQ, x.lb()));
                    } else {
                        const LC lc2 = tangente(x.lb(), func(x.lb()), deriv(x.lb()), env_type);
                        ce.add_linear_constraint(lc2);
                    }
                }
                // + middle ?
            } else {
                // default to centered form
                IntervalSide side = IntervalSide::Right;
                ce.add_linear_constraint(centered_form(x, func, deriv, env_type, side, prop.vertical_ub_tangente));
            }
        }
    }

    DECLARE_MAX_LIN_CONST(CONVEX_CONCAVE)
    template<typename Func, typename Deriv>
    ConvexEnvelope convex_concave_function(
        const Interval& x, const Func& func, const Deriv& deriv,
        const ConvexConcaveProperties& prop) {
        // acos-like functions
        if(x.ub() <= prop.inflection_point.lb()) {
            return convex_function(x, func, deriv, prop.vertical_lb_tangente, prop.vertical_ub_tangente);
        } else if(x.lb() >= prop.inflection_point.ub()) {
            return concave_function(x, func, deriv, prop.vertical_lb_tangente, prop.vertical_ub_tangente);
        } else {
            ConvexEnvelope ce(1);
            left_part(x, func, deriv, prop, EnvelopeType::Over, ce);
            right_part(x, func, deriv, prop, EnvelopeType::Under, ce);
            return ce;
        }
    }

    DECLARE_MAX_LIN_CONST(CONCAVE_CONVEX)
    template<typename Func, typename Deriv>
    ConvexEnvelope concave_convex_function(
        const Interval& x, const Func& func, const Deriv& deriv,
        const ConvexConcaveProperties& prop) {

        using LC = ConvexEnvelope::LinearConstraint;
        if(x.ub() <= prop.inflection_point.lb()) {
            return concave_function(x, func, deriv, prop.vertical_lb_tangente, prop.vertical_ub_tangente);
        } else if(x.lb() >= prop.inflection_point.ub()) {
            return convex_function(x, func, deriv, prop.vertical_lb_tangente, prop.vertical_ub_tangente);
        } else {
            ConvexEnvelope ce(1);
            left_part(x, func, deriv, prop, EnvelopeType::Under, ce);
            right_part(x, func, deriv, prop, EnvelopeType::Over, ce);
            return ce;
        }
    }

#undef DECLARE_MAX_LIN_CONST
} // end namespace convex_envelope




class ConvexEnvelopeVector {
public:
    ConvexEnvelopeVector(int n);

    int size() const;
    void resize(int n);
    ConvexEnvelopeVector subvector(int first_index, int last_index) const;
    const ConvexEnvelope& operator[](int i) const;
    ConvexEnvelope& operator[](int i);

private:
    std::vector<ConvexEnvelope> envelopes_;
};

class ConvexEnvelopeMatrix {
public:
    ConvexEnvelopeMatrix(int nb_rows, int nb_cols);

    const ConvexEnvelopeVector& row(int index) const;
    ConvexEnvelopeVector col(int index) const;
    int nb_rows() const;
    ConvexEnvelopeMatrix submatrix(int first_row, int last_row, int first_col, int last_col) const;
    const ConvexEnvelopeVector& operator[](int i) const;
    ConvexEnvelopeVector& operator[](int i);

private:
    std::vector<ConvexEnvelopeVector> rows_;
    int nb_rows_;
    int nb_cols_;
};

} // end namespace ibex


#endif  // __IBEX_CONVEX_ENVELOPE_H__