#pragma once

enum class LogLevel {
    // <summary>
    /// Internal system events that aren't necessarily observable from the outside.
    /// </summary>
    Debug,

    /// <summary>
    /// The lifeblood of operational intelligence - things happen.
    /// </summary>
    Info,

    /// <summary>
    /// Service is degraded or endangered.
    /// </summary>
    Warning,

    /// <summary>
    /// Functionality is unavailable, invariants are broken or data is lost.
    /// </summary>
    Error
};