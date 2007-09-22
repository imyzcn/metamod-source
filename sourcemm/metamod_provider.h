#ifndef _INCLUDE_METAMOD_SOURCE_SUPPORT_H_
#define _INCLUDE_METAMOD_SOURCE_SUPPORT_H_

#include <sourcehook/sourcehook.h>
#include <sourcehook/sh_memfuncinfo.h>
#include <iserverplugin.h>

namespace SourceMM
{
	enum
	{
		ConVarFlag_None,
		ConVarFlag_Notify,
		ConVarFlag_Replicated,
		ConVarFlag_SpOnly
	};

	enum ProvidedHooks
	{
		ProvidedHook_LevelInit = 0,			/**< IServerGameDLL::LevelInit */
		ProvidedHook_LevelShutdown = 1,		/**< IServerGameDLL::LevelShutdown */
		ProvidedHook_DLLInit = 2,			/**< IServerGameDLL::DLLInit */
		ProvidedHook_DLLShutdown = 3,		/**< IServerGameDLL::DLLShutdown */
		ProvidedHook_GameInit = 4,			/**< IServerGameDLL::GameInit */
	};

	/**
	 * @brief Abstracts command information, since the new engine fixes the 
	 * re-entrancy problems in the tokenization system.
	 */
	class IMetamodSourceCommandInfo
	{
	public:
		/**
		 * @brief Returns the argument count such that arguments 
		 * 1 to N are valid arguments, and 0 is the command name.
		 *
		 * @return					Argument count.
		 */
		virtual unsigned int GetArgCount() =0;

		/**
		 * @brief Returns the string of an argument number.
		 *
		 * @param num				Argument number.
		 * @return					Argument text.
		 */
		virtual const char *GetArg(unsigned int num) =0;

		/**
		 * @brief Returns the entire command argument string.
		 *
		 * @return					Argument string.
		 */
		virtual const char *GetArgString() =0;
	};

	typedef bool (*METAMOD_COMMAND)(edict_t *pEdict, IMetamodSourceCommandInfo *info);

	class IMetamodSourceProvider
	{
	public:
		/**
		 * @brief Returns whether source engine build is compatible.
		 *
		 * @param build				Source engine build.
		 * @return					True if compatible, false otherwise.
		 */
		virtual bool IsSourceEngineBuildCompatible(int build) =0;

		/**
		 * @brief Retrieves hook information for each callback.  Each hook 
		 * must be implemented.
		 *
		 * @param hook				Hook information to provide.
		 * @param pInfo				Non-NULL pointer to fill with information 
		 * 							about the hook's virtual location.
		 * @return					True if supported, false to fail, which 
		 * 							will cause Metamod:Source to fail.
		 */
		virtual bool GetHookInfo(ProvidedHooks hook, SourceHook::MemFuncInfo *pInfo) =0;

		/**
		 * @brief Logs a message via IVEngineServer::LogPrint.
		 * 
		 * @param buffer			Buffer to print.
		 * @return					True on success, false if not supported, 
		 * 							or IVEngineServer is not yet known.
		 */
		virtual bool LogMessage(const char *buffer) =0;

		/**
		 * @brief Returns the command line value of a parameter.  If ICvar 
		 * is not yet loaded, this uses an equivalent function.
		 *
		 * @param key				Parameter to look up.
		 * @param val				Default string to return if none found.
		 * @return					Parameter value.
		 */
		virtual const char *GetCommandLineValue(const char *key, const char *defval) =0;

		/**
		 * @brief Prints a string to the remote server console. 
		 * 
		 * Note: new lines are not appended.
		 *
		 * @param msg			 	Message string.
		 */
		virtual void ConsolePrint(const char *msg) =0;

		/**
		 * @brief Returns whether remote printing is available.
		 *
		 * @return					True if remote printing is available, 
		 * 							otherwise returns false.
		 */
		virtual bool IsRemotePrintingAvailable() =0;

		/**
		 * @brief Prints text in the specified client's console.
		 *
		 * @param client			Client edict pointer.
		 * @param msg				Message string.
		 */
		virtual void ClientConsolePrint(edict_t *client, const char *msg) =0;

		/**
		 * @brief Returns a server plugin helper for the given interface 
		 * string.
		 *
		 * @param iface				Interface version
		 * @return					IServerPluginCallbacks pointer.
		 */
		virtual IServerPluginCallbacks *GetVSPCallbacks(const char *iface) =0;

		/**
		 * @brief Halts the server with a fatal error message.
		 *
		 * @param fmt				Formatted message string.
		 * @param ...				Format parameters.
		 */
		virtual void DisplayError(const char *fmt, ...) =0;

		/**
		 * @brief Sends the server a warning message.
		 *
		 * @param fmt				Formatted message string.
		 * @param ...				Format parameters.
		 */
		virtual void DisplayWarning(const char *fmt, ...) =0;

		/**
		 * @brief Attempts to notify the provider of the gamedll version being 
		 * used.
		 *
		 * @param iface				Interface string.
		 * @return					Version number on success, 0 otherwise.
		 */
		virtual int TryServerGameDLL(const char *iface) =0;

		/**
		 * @brief Attempts to notify the provider of the gameclients version 
		 * being used.
		 *
		 * @param iface				Interface string.
		 * @return					Version number on success, 0 otherwise.
		 */
		virtual int TryServerGameClients(const char *iface) =0;

		/**
		 * @brief Notifies the provider that the DLLInit pre-hook is almost done.
		 */
		virtual void Notify_DLLInit_Pre() =0;

		/**
		 * @brief Wrapper around IVEngineServer::ServerCommand()
		 *
		 * @param cmd				Command string.
		 */
		virtual void ServerCommand(const char *cmd) =0;

		/**
		 * @brief Creates a ConVar pointer.
		 *
		 * @param name				ConVar name.
		 * @param defval			Default value string.
		 * @param flags				ConVar flags.
		 * @param help				Help text.
		 * @return					ConVar pointer.
		 */
		virtual ConVar *CreateConVar(const char *name, 
			const char *defval, 
			const char *help,
			int flags) =0;

		/**
		 * @brief Returns the string value of a ConVar.
		 *
		 * @param convar			ConVar pointer.
		 * @return					String value.
		 */
		virtual const char *GetConVarString(ConVar *convar) =0;

		/**
		 * @brief Creates a console command.
		 *
		 * @param name				Command name.
		 * @param callback			Callback pointer.
		 * @param help				Help text.
		 */
		virtual void CreateCommand(const char *name,
			METAMOD_COMMAND callback,
			const char *help) =0;

		/**
		 * @brief Sets the ClientCommand handler.
		 *
		 * @param callback			Callback pointer.
		 */
		virtual void SetClientCommandHandler(METAMOD_COMMAND callback) =0;

		/**
		 * @brief Retrieves the game description.
		 *
		 * @return					Game description.
		 */
		virtual const char *GetGameDescription() =0;

		/**
		 * @brief Returns the ConCommandBase accessor.
		 *
		 * @return				An IConCommandBaseAccessor pointer.
		 */
		virtual IConCommandBaseAccessor *GetConCommandBaseAccessor() =0;

		/**
		 * @brief Registers a ConCommandBase.
		 *
		 * @param pCommand		ConCommandBase to register.
		 * @return				True if successful, false otherwise.
		 */
		virtual bool RegisterConCommandBase(ConCommandBase *pCommand) =0;

		/**
		 * @brief Unregisters a ConCommandBase.
		 *
		 * @param pCommand		ConCommandBase to unlink.
		 */
		virtual void UnregisterConCommandBase(ConCommandBase *pCommand) =0;

		/**
		 * @brief Returns whether a ConCommandBase is a command or not.
		 *
		 * @param pCommand		ConCommandBase pointer.
		 * @return				True if a command, false otherwise.
		 */
		virtual bool IsConCommandBaseACommand(ConCommandBase *pCommand) =0;

		/**
		 * @brief Returns the number of user messages in the GameDLL.
		 *
		 * @return				Number of user messages, or -1 if SourceMM has 
		 *						failed to get user message list.
		 */
		virtual int GetUserMessageCount() =0;

		/**
		 * @brief Returns the index of the specified user message.
		 *
		 * @param name			User message name.
		 * @param size			Optional pointer to store size of user message.
		 * @return				Message index, or -1 on failure.
		 */
		virtual int FindUserMessage(const char *name, int *size=NULL) =0;

		/**
		 * @brief Returns the name of the specified user message.
		 *
		 * @param index			User message index.
		 * @param size			Optional pointer to store size of user message.
		 * @return				Message name, or NULL on failure.
		 */
		virtual const char *GetUserMessage(int index, int *size=NULL) =0;
	};
};

#endif //_INCLUDE_METAMOD_SOURCE_SUPPORT_H_

