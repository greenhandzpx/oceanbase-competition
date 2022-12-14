// Code generated by entc, DO NOT EDIT.

package ent

import (
	"context"
	"fmt"
	"log"

	"github.com/oceanbase/configserver/ent/migrate"

	"github.com/oceanbase/configserver/ent/obcluster"

	"entgo.io/ent/dialect"
	"entgo.io/ent/dialect/sql"
)

// Client is the client that holds all ent builders.
type Client struct {
	config
	// Schema is the client for creating, migrating and dropping schema.
	Schema *migrate.Schema
	// ObCluster is the client for interacting with the ObCluster builders.
	ObCluster *ObClusterClient
}

// NewClient creates a new client configured with the given options.
func NewClient(opts ...Option) *Client {
	cfg := config{log: log.Println, hooks: &hooks{}}
	cfg.options(opts...)
	client := &Client{config: cfg}
	client.init()
	return client
}

func (c *Client) init() {
	c.Schema = migrate.NewSchema(c.driver)
	c.ObCluster = NewObClusterClient(c.config)
}

// Open opens a database/sql.DB specified by the driver name and
// the data source name, and returns a new client attached to it.
// Optional parameters can be added for configuring the client.
func Open(driverName, dataSourceName string, options ...Option) (*Client, error) {
	switch driverName {
	case dialect.MySQL, dialect.Postgres, dialect.SQLite:
		drv, err := sql.Open(driverName, dataSourceName)
		if err != nil {
			return nil, err
		}
		return NewClient(append(options, Driver(drv))...), nil
	default:
		return nil, fmt.Errorf("unsupported driver: %q", driverName)
	}
}

// Tx returns a new transactional client. The provided context
// is used until the transaction is committed or rolled back.
func (c *Client) Tx(ctx context.Context) (*Tx, error) {
	if _, ok := c.driver.(*txDriver); ok {
		return nil, fmt.Errorf("ent: cannot start a transaction within a transaction")
	}
	tx, err := newTx(ctx, c.driver)
	if err != nil {
		return nil, fmt.Errorf("ent: starting a transaction: %w", err)
	}
	cfg := c.config
	cfg.driver = tx
	return &Tx{
		ctx:       ctx,
		config:    cfg,
		ObCluster: NewObClusterClient(cfg),
	}, nil
}

// BeginTx returns a transactional client with specified options.
func (c *Client) BeginTx(ctx context.Context, opts *sql.TxOptions) (*Tx, error) {
	if _, ok := c.driver.(*txDriver); ok {
		return nil, fmt.Errorf("ent: cannot start a transaction within a transaction")
	}
	tx, err := c.driver.(interface {
		BeginTx(context.Context, *sql.TxOptions) (dialect.Tx, error)
	}).BeginTx(ctx, opts)
	if err != nil {
		return nil, fmt.Errorf("ent: starting a transaction: %w", err)
	}
	cfg := c.config
	cfg.driver = &txDriver{tx: tx, drv: c.driver}
	return &Tx{
		ctx:       ctx,
		config:    cfg,
		ObCluster: NewObClusterClient(cfg),
	}, nil
}

// Debug returns a new debug-client. It's used to get verbose logging on specific operations.
//
//	client.Debug().
//		ObCluster.
//		Query().
//		Count(ctx)
//
func (c *Client) Debug() *Client {
	if c.debug {
		return c
	}
	cfg := c.config
	cfg.driver = dialect.Debug(c.driver, c.log)
	client := &Client{config: cfg}
	client.init()
	return client
}

// Close closes the database connection and prevents new queries from starting.
func (c *Client) Close() error {
	return c.driver.Close()
}

// Use adds the mutation hooks to all the entity clients.
// In order to add hooks to a specific client, call: `client.Node.Use(...)`.
func (c *Client) Use(hooks ...Hook) {
	c.ObCluster.Use(hooks...)
}

// ObClusterClient is a client for the ObCluster schema.
type ObClusterClient struct {
	config
}

// NewObClusterClient returns a client for the ObCluster from the given config.
func NewObClusterClient(c config) *ObClusterClient {
	return &ObClusterClient{config: c}
}

// Use adds a list of mutation hooks to the hooks stack.
// A call to `Use(f, g, h)` equals to `obcluster.Hooks(f(g(h())))`.
func (c *ObClusterClient) Use(hooks ...Hook) {
	c.hooks.ObCluster = append(c.hooks.ObCluster, hooks...)
}

// Create returns a create builder for ObCluster.
func (c *ObClusterClient) Create() *ObClusterCreate {
	mutation := newObClusterMutation(c.config, OpCreate)
	return &ObClusterCreate{config: c.config, hooks: c.Hooks(), mutation: mutation}
}

// CreateBulk returns a builder for creating a bulk of ObCluster entities.
func (c *ObClusterClient) CreateBulk(builders ...*ObClusterCreate) *ObClusterCreateBulk {
	return &ObClusterCreateBulk{config: c.config, builders: builders}
}

// Update returns an update builder for ObCluster.
func (c *ObClusterClient) Update() *ObClusterUpdate {
	mutation := newObClusterMutation(c.config, OpUpdate)
	return &ObClusterUpdate{config: c.config, hooks: c.Hooks(), mutation: mutation}
}

// UpdateOne returns an update builder for the given entity.
func (c *ObClusterClient) UpdateOne(oc *ObCluster) *ObClusterUpdateOne {
	mutation := newObClusterMutation(c.config, OpUpdateOne, withObCluster(oc))
	return &ObClusterUpdateOne{config: c.config, hooks: c.Hooks(), mutation: mutation}
}

// UpdateOneID returns an update builder for the given id.
func (c *ObClusterClient) UpdateOneID(id int) *ObClusterUpdateOne {
	mutation := newObClusterMutation(c.config, OpUpdateOne, withObClusterID(id))
	return &ObClusterUpdateOne{config: c.config, hooks: c.Hooks(), mutation: mutation}
}

// Delete returns a delete builder for ObCluster.
func (c *ObClusterClient) Delete() *ObClusterDelete {
	mutation := newObClusterMutation(c.config, OpDelete)
	return &ObClusterDelete{config: c.config, hooks: c.Hooks(), mutation: mutation}
}

// DeleteOne returns a delete builder for the given entity.
func (c *ObClusterClient) DeleteOne(oc *ObCluster) *ObClusterDeleteOne {
	return c.DeleteOneID(oc.ID)
}

// DeleteOneID returns a delete builder for the given id.
func (c *ObClusterClient) DeleteOneID(id int) *ObClusterDeleteOne {
	builder := c.Delete().Where(obcluster.ID(id))
	builder.mutation.id = &id
	builder.mutation.op = OpDeleteOne
	return &ObClusterDeleteOne{builder}
}

// Query returns a query builder for ObCluster.
func (c *ObClusterClient) Query() *ObClusterQuery {
	return &ObClusterQuery{
		config: c.config,
	}
}

// Get returns a ObCluster entity by its id.
func (c *ObClusterClient) Get(ctx context.Context, id int) (*ObCluster, error) {
	return c.Query().Where(obcluster.ID(id)).Only(ctx)
}

// GetX is like Get, but panics if an error occurs.
func (c *ObClusterClient) GetX(ctx context.Context, id int) *ObCluster {
	obj, err := c.Get(ctx, id)
	if err != nil {
		panic(err)
	}
	return obj
}

// Hooks returns the client hooks.
func (c *ObClusterClient) Hooks() []Hook {
	return c.hooks.ObCluster
}
