// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_API_BLUETOOTH_BLUETOOTH_PRIVATE_API_H_
#define EXTENSIONS_BROWSER_API_BLUETOOTH_BLUETOOTH_PRIVATE_API_H_

#include "base/callback_forward.h"
#include "base/macros.h"
#include "device/bluetooth/bluetooth_device.h"
#include "extensions/browser/api/bluetooth/bluetooth_extension_function.h"
#include "extensions/browser/browser_context_keyed_api_factory.h"
#include "extensions/browser/event_router.h"

namespace device {
class BluetoothAdapter;
}

namespace extensions {

class BluetoothApiPairingDelegate;

// The profile-keyed service that manages the bluetoothPrivate extension API.
class BluetoothPrivateAPI : public BrowserContextKeyedAPI,
                            public EventRouter::Observer {
 public:
  static BrowserContextKeyedAPIFactory<BluetoothPrivateAPI>*
      GetFactoryInstance();

  explicit BluetoothPrivateAPI(content::BrowserContext* context);
  ~BluetoothPrivateAPI() override;

  // KeyedService implementation.
  void Shutdown() override;

  // EventRouter::Observer implementation.
  void OnListenerAdded(const EventListenerInfo& details) override;
  void OnListenerRemoved(const EventListenerInfo& details) override;

  // BrowserContextKeyedAPI implementation.
  static const char* service_name() { return "BluetoothPrivateAPI"; }
  static const bool kServiceRedirectedInIncognito = true;
  static const bool kServiceIsNULLWhileTesting = true;

 private:
  friend class BrowserContextKeyedAPIFactory<BluetoothPrivateAPI>;

  content::BrowserContext* browser_context_;
};

namespace api {

class BluetoothPrivateSetAdapterStateFunction
    : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.setAdapterState",
                             BLUETOOTHPRIVATE_SETADAPTERSTATE)
  BluetoothPrivateSetAdapterStateFunction();

 private:
  ~BluetoothPrivateSetAdapterStateFunction() override;

  base::Closure CreatePropertySetCallback(const std::string& property_name);
  base::Closure CreatePropertyErrorCallback(const std::string& property_name);
  void OnAdapterPropertySet(const std::string& property);
  void OnAdapterPropertyError(const std::string& property);
  void SendError();

  // BluetoothExtensionFunction overrides:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

  // Set of expected adapter properties to be changed.
  std::set<std::string> pending_properties_;

  // Set of adapter properties that were not set successfully.
  std::set<std::string> failed_properties_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivateSetAdapterStateFunction);
};

class BluetoothPrivateSetPairingResponseFunction
    : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.setPairingResponse",
                             BLUETOOTHPRIVATE_SETPAIRINGRESPONSE)
  BluetoothPrivateSetPairingResponseFunction();
  // BluetoothExtensionFunction overrides:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  ~BluetoothPrivateSetPairingResponseFunction() override;
  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivateSetPairingResponseFunction);
};

class BluetoothPrivateDisconnectAllFunction
    : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.disconnectAll",
                             BLUETOOTHPRIVATE_DISCONNECTALL);
  BluetoothPrivateDisconnectAllFunction();

  // BluetoothExtensionFunction overrides:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  ~BluetoothPrivateDisconnectAllFunction() override;

  void OnSuccessCallback();
  void OnErrorCallback(scoped_refptr<device::BluetoothAdapter> adapter,
                       const std::string& device_address);

  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivateDisconnectAllFunction);
};

class BluetoothPrivateForgetDeviceFunction : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.forgetDevice",
                             BLUETOOTHPRIVATE_FORGETDEVICE);
  BluetoothPrivateForgetDeviceFunction();

  // BluetoothExtensionFunction overrides:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  ~BluetoothPrivateForgetDeviceFunction() override;

  void OnSuccessCallback();
  void OnErrorCallback(scoped_refptr<device::BluetoothAdapter> adapter,
                       const std::string& device_address);

  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivateForgetDeviceFunction);
};

class BluetoothPrivateSetDiscoveryFilterFunction
    : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.setDiscoveryFilter",
                             BLUETOOTHPRIVATE_SETDISCOVERYFILTER)

 protected:
  ~BluetoothPrivateSetDiscoveryFilterFunction() override {}

  // BluetoothExtensionFunction:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  void OnSuccessCallback();
  void OnErrorCallback();
};

class BluetoothPrivateConnectFunction : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.connect",
                             BLUETOOTHPRIVATE_CONNECT)
  BluetoothPrivateConnectFunction();

  // BluetoothExtensionFunction:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  ~BluetoothPrivateConnectFunction() override;

  void OnSuccessCallback();
  void OnErrorCallback(device::BluetoothDevice::ConnectErrorCode error);

  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivateConnectFunction);
};

class BluetoothPrivatePairFunction : public BluetoothExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bluetoothPrivate.pair", BLUETOOTHPRIVATE_PAIR)
  BluetoothPrivatePairFunction();

  // BluetoothExtensionFunction:
  bool DoWork(scoped_refptr<device::BluetoothAdapter> adapter) override;

 private:
  ~BluetoothPrivatePairFunction() override;

  void OnSuccessCallback();
  void OnErrorCallback(device::BluetoothDevice::ConnectErrorCode error);

  DISALLOW_COPY_AND_ASSIGN(BluetoothPrivatePairFunction);
};

}  // namespace api

}  // namespace extensions

#endif  // EXTENSIONS_BROWSER_API_BLUETOOTH_BLUETOOTH_PRIVATE_API_H_
