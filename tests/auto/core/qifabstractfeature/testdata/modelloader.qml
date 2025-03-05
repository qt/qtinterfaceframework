// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import testfeature

Item {
    signal asyncLoaderLoaded
    signal asyncLoaderOverridenLoaded
    Loader {
        sourceComponent: TestFeatureModel {
            objectName: "inSyncLoader"
        }
    }
    Loader {
        sourceComponent: TestFeatureModel {
            objectName: "inAsyncLoader"
        }
        asynchronous: true
        onLoaded: asyncLoaderLoaded()
    }
    Loader {
        sourceComponent: TestFeatureModel {
            objectName: "inAsyncLoaderOverridden"
            asynchronousBackendLoading: false
        }
        asynchronous: true
        onLoaded: asyncLoaderOverridenLoaded()
    }
}
