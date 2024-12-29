#pragma once

#include <login_matchmaking_game_shared/userMatchmakingSerialization.hxx>
// clang-format off

namespace user_matchmaking{
static boost::hana::tuple<
JoinChannel,
JoinChannelSuccess,
JoinChannelError,
CreateAccount,
CreateAccountCancel,
CreateAccountSuccess,
CreateAccountError,
LoginAccount,
LoginAccountCancel,
LoginAccountSuccess,
LoginAccountError,
LogoutAccount,
LogoutAccountSuccess,
LogoutAccountError,
BroadCastMessage,
BroadCastMessageSuccess,
BroadCastMessageError,
LeaveChannel,
LeaveChannelSuccess,
LeaveChannelError,
Message,
CreateGameLobby,
CreateGameLobbySuccess,
CreateGameLobbyError,
JoinGameLobby,
JoinGameLobbySuccess,
JoinGameLobbyError,
GameOptionError,
UserInGameLobby,
UsersInGameLobby,
SetMaxUserSizeInCreateGameLobby,
SetMaxUserSizeInCreateGameLobbyError,
MaxUserSizeInCreateGameLobby,
LeaveGameLobby,
LeaveGameLobbySuccess,
LeaveGameLobbyError,
WantToRelog,
RelogTo,
RelogToCreateGameLobbySuccess,
RelogToGameSuccess,
RelogToError,
CreateGame,
CreateGameError,
StartGame,
StartGameError,
JoinMatchMakingQueue,
JoinMatchMakingQueueSuccess,
JoinMatchMakingQueueError,
AskIfUserWantsToJoinGame,
AskIfUserWantsToJoinGameTimeOut,
GameStartCanceled,
GameStartCanceledRemovedFromQueue,
WantsToJoinGame,
WantsToJoinGameError,
LeaveQuickGameQueue,
LeaveQuickGameQueueSuccess,
LeaveQuickGameQueueError,
LoginAsGuest,
LoginAsGuestSuccess,
RatingChanged,
UnhandledMessageError,
user_matchmaking_game::GameOptionAsString,
ConnectGameError,
ProxyStarted,
ProxyStopped,
GetMatchmakingLogic,
MatchmakingLogic,
GetUserStatistics,
GetTopRatedPlayers
  >  const  userMatchmaking{};

// clang-format on
}
