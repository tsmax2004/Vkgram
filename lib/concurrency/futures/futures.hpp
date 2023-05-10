#pragma once

// Constructors:
#include <futures/make/contract.hpp>
#include <futures/make/failure.hpp>
#include <futures/make/just.hpp>
#include <futures/make/submit.hpp>
#include <futures/make/value.hpp>

// Destructors:
#include <futures/terminate/get.hpp>
#include <futures/terminate/detach.hpp>
#include <futures/terminate/await.hpp>

// Combinators
#include <futures/combine/seq/and_then.hpp>
#include <futures/combine/seq/flat_map.hpp>
#include <futures/combine/seq/flatten.hpp>
#include <futures/combine/seq/map.hpp>
#include <futures/combine/seq/or_else.hpp>
#include <futures/combine/seq/via.hpp>

#include <futures/combine/par/first.hpp>
#include <futures/combine/par/all.hpp>

// Syntax
#include <futures/syntax/both.hpp>
#include <futures/syntax/or.hpp>
#include <futures/syntax/pipe.hpp>