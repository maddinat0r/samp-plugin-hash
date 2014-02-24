/*
 * Copyright (C) 2014 Mellnik
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(WIN32)
#pragma comment(lib, "cryptlib")
#endif

#include <cstdlib>

#include "plugin.h"
#include "callback.h"
#include "natives.h"
#include "main.h"

logprintf_t logprintf;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	g_Plugin = new Plugin();
	g_Callback = new Callback();
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	delete g_Callback;
	delete g_Plugin;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	g_Callback->ProcessTick();
	g_Callback->ProcessTask();
}

AMX_NATIVE_INFO hash_natives[] =
{
	// Hashing
	{"sha256", Native::sha256},
	{"sha384", Native::sha384},
	{"sha512", Native::sha512},
	{"whirlpool", Native::whirlpool},
	{"ripemd160", Native::ripemd160},
	{"ripemd256", Native::ripemd256},
	{"ripemd320", Native::ripemd320},

	// PBKDF2
	{"hash_generate", Native::hash_generate},
	{"hash_retrieve", Native::hash_retrieve},
	{"hash_validate", Native::hash_validate},
	{"hash_is_equal", Native::hash_is_equal},
	{"hash_unprocessed", Native::hash_unprocessed},
	{"hash_exec_time", Native::hash_exec_time},
	{"hash_thread_limit", Native::hash_thread_limit},

	// Non-cryptographic algorithms
	{"base64_encode", Native::base64_encode},
	{"base64_decode", Native::base64_decode},
	{"hex_encode", Native::hex_encode},
	{"hex_decode", Native::hex_decode},

	// Pseudo random generators
	{"random_int", Native::random_int},
	{"random_string", Native::random_string},

	// Checksums
	{"md5sum", Native::md5sum},
	{"sha1sum", Native::sha1sum},
	{"sha256sum", Native::sha256sum},
	{"sha384sum", Native::sha384sum},
	{"sha512sum", Native::sha512sum},
	{"wpsum", Native::wpsum},

	// Length-constant comparison
	{"slow_equals", Native::slow_equals},

	{NULL, NULL}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	g_Plugin->AddAmx(amx);
	return amx_Register(amx, hash_natives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	g_Plugin->EraseAmx(amx);
	return AMX_ERR_NONE;
}