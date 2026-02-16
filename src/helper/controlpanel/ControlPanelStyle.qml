// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

pragma Singleton
import QtQuick

/*!
    \qmltype ControlPanelStyle
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-singletons
    \brief A singleton that provides centralized style constants for
    all control panel components.

    ControlPanelStyle defines the color palette, font sizes, spacing
    values, border widths, animation durations, and icon glyphs used
    throughout the simulation control panel. All properties are
    read-only and should be referenced by other QML types rather than
    hard-coding style values.

    \section1 Usage

    \qml
    Rectangle {
        color: ControlPanelStyle.backgroundGray
        border.color: ControlPanelStyle.borderLight
        border.width: ControlPanelStyle.borderWidth
        radius: ControlPanelStyle.radius
    }
    \endqml
*/
QtObject {
    id: root

    readonly property int maxInt32: 2147483647
    readonly property int minInt32: -2147483648

    // Primary Colors
    readonly property color primaryBlue: "#0078d4"
    readonly property color primaryBlueDark: "#005a9e"
    readonly property color primaryBlueLight: "#e7f3ff"

    readonly property color accentGreen: "#28a745"

    // Background Colors
    readonly property color backgroundWhite: "#ffffff"
    readonly property color backgroundGray: "#f8f9fa"
    readonly property color backgroundDarkGray: "#e9ecef"

    // Border Colors
    readonly property color borderLight: "#dee2e6"

    // Text Colors
    readonly property color textPrimary: "#212529"
    readonly property color textSecondary: "#6c757d"

    // Interactive Colors
    readonly property color hoverGray: "#f0f0f0"

    // Badge Colors
    readonly property color badgeNonZoned: "#d4edda"
    readonly property color badgeNonZonedText: "#155724"
    readonly property color badgeZoned: "#cce5ff"
    readonly property color badgeZonedText: "#004085"
    readonly property color badgeReadonly: "#e9ecef"
    readonly property color badgeReadonlyText: "#6c757d"

    readonly property int fontSizeH1: 14
    readonly property int fontSizeH2: 13
    readonly property int fontSizeH3: 12
    readonly property int fontSizeBody: 12
    readonly property int fontSizeSmall: 11
    readonly property int fontSizeCaption: 10
    readonly property int fontSizeTiny: 9
    readonly property int fontSizeMicro: 8

    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 12
    readonly property int controlHeight: 32

    readonly property int radius: 4

    readonly property int borderWidth: 1
    readonly property int borderWidthFocus: 2

    readonly property int animationDuration: 150

    //icons
    readonly property int iconSize : 14
    readonly property int iconSizeMini : 8
    readonly property int iconSizeLarge : 32
    readonly property string iconSearch: "\u2315"              //  search icon
    readonly property string iconClose: "\u2715"               //  close icon
    readonly property string iconMenu: "\u22EE"                //  Vertical ellipsis
    readonly property string iconWarning: "\u2298"             //  Warning sign
    readonly property string iconGear: "\u2699"                //  Gear
    readonly property string iconDropdown: "\u25BC"            //  Down arrow
    readonly property string iconUp: "\u25B2"                  //  Up arrow
    readonly property string iconZoned: "\u25A3"               //  Square with fill
    readonly property string iconNonZoned: "\u25A1"            //  Empty square
    readonly property string iconAllProperties: "\u25A0"       //  Solid square

}
