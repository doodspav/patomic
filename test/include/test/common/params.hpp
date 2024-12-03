#ifndef PATOMIC_TEST_COMMON_PARAMS_HPP
#define PATOMIC_TEST_COMMON_PARAMS_HPP

#include <patomic/api/ids.h>
#include <patomic/api/memory_order.h>

#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

namespace test
{


/// @brief
///   Combination of parameters for creating a patomic implicit struct.
struct ParamsImplicit
{
    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, std::size_t, patomic_memory_order_t, unsigned int
    >;

    /// @brief Construct from tuple of members.
    explicit ParamsImplicit(TupleT tup) noexcept;

    /// @brief Default constructor.
    ParamsImplicit() noexcept = default;

    /// @brief Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief Create all combinations of params for tests.
    static std::vector<ParamsImplicit>
    combinations();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for store operations.
    static std::vector<ParamsImplicit>
    combinations_store();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for load operations.
    static std::vector<ParamsImplicit>
    combinations_load();
};


/// @brief
///   Combination of parameters for creating a patomic explicit struct.
struct ParamsExplicit
{
    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, std::size_t, patomic_memory_order_t, unsigned int
    >;

    /// @brief Construct from tuple of members.
    explicit ParamsExplicit(TupleT tup) noexcept;

    /// @brief Default constructor.
    ParamsExplicit() noexcept = default;

    /// @brief Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief Create all combinations of params for tests.
    static std::vector<ParamsExplicit>
    combinations();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for store operations.
    static std::vector<ParamsExplicit>
    combinations_store();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for load operations.
    static std::vector<ParamsExplicit>
    combinations_load();
};


/// @brief
///   Combination of parameters for creating a patomic transaction struct.
struct ParamsTransaction
{
    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Widths to use for operation.
    std::vector<std::size_t> widths {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, unsigned int, std::vector<std::size_t>
    >;

    /// @brief Construct from tuple of members.
    explicit ParamsTransaction(TupleT tup) noexcept;

    /// @brief Default constructor.
    ParamsTransaction() noexcept = default;

    /// @brief Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief Create all combinations of params for tests.
    static std::vector<ParamsTransaction>
    combinations();
};


class Params
{
public:
    /// @brief Default constructor.
    Params() noexcept;

    /// @brief Destructor.
    ~Params() noexcept;

    /// @brief Construct from implicit params.
    explicit Params(ParamsImplicit imp) noexcept;

    /// @brief Construct from explicit params.
    explicit Params(ParamsExplicit exp) noexcept;

    /// @brief Construct from transaction params.
    explicit Params(ParamsTransaction tsx) noexcept;

    /// @brief Copy constructible.
    Params(const Params& other);

    /// @brief Move constructible.
    Params(Params&& other) noexcept;

    /// @brief Copy assignable.
    Params&
    operator=(const Params& other);

    /// @brief Move assignable.
    Params&
    operator=(Params&& other) noexcept;

    /// @brief Get the internal implicit params.
    /// @warning Undefined behaviour if implicit params are not contained.
    ParamsImplicit&
    imp() noexcept;

    /// @brief Get the internal implicit params.
    /// @warning Undefined behaviour if implicit params are not contained.
    const ParamsImplicit&
    imp() const noexcept;

    /// @brief Get the internal explicit params.
    /// @warning Undefined behaviour if explicit params are not contained.
    ParamsExplicit&
    exp() noexcept;

    /// @brief Get the internal explicit params.
    /// @warning Undefined behaviour if explicit params are not contained.
    const ParamsExplicit&
    exp() const noexcept;

    /// @brief Get the internal implicit params.
    /// @warning Undefined behaviour if implicit params are not contained.
    ParamsTransaction&
    tsx() noexcept;

    /// @brief Get the internal implicit params.
    /// @warning Undefined behaviour if implicit params are not contained.
    const ParamsTransaction&
    tsx() const noexcept;

private:
    /// @brief Tag type.
    enum class Type
    {
        NONE = 0,
        IMP,
        EXP,
        TSX

    } m_type {};

    /// @brief Union of params.
    union ParamsUnion
    {
        // members
        struct {} none {};
        ParamsImplicit imp;
        ParamsExplicit exp;
        ParamsTransaction tsx;

        // constructor/destructor which would otherwise be deleted
        ParamsUnion() noexcept {}
        ~ParamsUnion() noexcept {}

    } m_union;
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_PARAMS_HPP
