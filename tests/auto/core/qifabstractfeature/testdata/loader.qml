// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import testfeature

Item {
    signal asyncLoaderLoaded
    signal asyncLoaderOverridenLoaded
    Loader {
        sourceComponent: TestFeature {
            objectName: "inSyncLoader"
        }
    }
    Loader {
        sourceComponent: TestFeature {
            objectName: "inAsyncLoader"
        }
        asynchronous: true
        onLoaded: asyncLoaderLoaded()
    }
    Loader {
        sourceComponent: TestFeature {
            objectName: "inAsyncLoaderOverridden"
            asynchronousBackendLoading: false
        }
        asynchronous: true
        onLoaded: asyncLoaderOverridenLoaded()
    }
}
