// Code generated by entc, DO NOT EDIT.

package ent

import (
	"context"
	"fmt"

	"entgo.io/ent/dialect/sql"
	"entgo.io/ent/dialect/sql/sqlgraph"
	"entgo.io/ent/schema/field"
	"github.com/oceanbase/configserver/ent/obcluster"
	"github.com/oceanbase/configserver/ent/predicate"
)

// ObClusterDelete is the builder for deleting a ObCluster entity.
type ObClusterDelete struct {
	config
	hooks    []Hook
	mutation *ObClusterMutation
}

// Where appends a list predicates to the ObClusterDelete builder.
func (ocd *ObClusterDelete) Where(ps ...predicate.ObCluster) *ObClusterDelete {
	ocd.mutation.Where(ps...)
	return ocd
}

// Exec executes the deletion query and returns how many vertices were deleted.
func (ocd *ObClusterDelete) Exec(ctx context.Context) (int, error) {
	var (
		err      error
		affected int
	)
	if len(ocd.hooks) == 0 {
		affected, err = ocd.sqlExec(ctx)
	} else {
		var mut Mutator = MutateFunc(func(ctx context.Context, m Mutation) (Value, error) {
			mutation, ok := m.(*ObClusterMutation)
			if !ok {
				return nil, fmt.Errorf("unexpected mutation type %T", m)
			}
			ocd.mutation = mutation
			affected, err = ocd.sqlExec(ctx)
			mutation.done = true
			return affected, err
		})
		for i := len(ocd.hooks) - 1; i >= 0; i-- {
			if ocd.hooks[i] == nil {
				return 0, fmt.Errorf("ent: uninitialized hook (forgotten import ent/runtime?)")
			}
			mut = ocd.hooks[i](mut)
		}
		if _, err := mut.Mutate(ctx, ocd.mutation); err != nil {
			return 0, err
		}
	}
	return affected, err
}

// ExecX is like Exec, but panics if an error occurs.
func (ocd *ObClusterDelete) ExecX(ctx context.Context) int {
	n, err := ocd.Exec(ctx)
	if err != nil {
		panic(err)
	}
	return n
}

func (ocd *ObClusterDelete) sqlExec(ctx context.Context) (int, error) {
	_spec := &sqlgraph.DeleteSpec{
		Node: &sqlgraph.NodeSpec{
			Table: obcluster.Table,
			ID: &sqlgraph.FieldSpec{
				Type:   field.TypeInt,
				Column: obcluster.FieldID,
			},
		},
	}
	if ps := ocd.mutation.predicates; len(ps) > 0 {
		_spec.Predicate = func(selector *sql.Selector) {
			for i := range ps {
				ps[i](selector)
			}
		}
	}
	return sqlgraph.DeleteNodes(ctx, ocd.driver, _spec)
}

// ObClusterDeleteOne is the builder for deleting a single ObCluster entity.
type ObClusterDeleteOne struct {
	ocd *ObClusterDelete
}

// Exec executes the deletion query.
func (ocdo *ObClusterDeleteOne) Exec(ctx context.Context) error {
	n, err := ocdo.ocd.Exec(ctx)
	switch {
	case err != nil:
		return err
	case n == 0:
		return &NotFoundError{obcluster.Label}
	default:
		return nil
	}
}

// ExecX is like Exec, but panics if an error occurs.
func (ocdo *ObClusterDeleteOne) ExecX(ctx context.Context) {
	ocdo.ocd.ExecX(ctx)
}
