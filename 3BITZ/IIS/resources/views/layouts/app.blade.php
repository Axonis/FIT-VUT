<!DOCTYPE html>
<html lang="{{ app()->getLocale() }}">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="Bug Tracker Information System for course IIS at FIT VUT Brno">
    <meta name="author" content="Jozef Urbanovský (xurban66), Adrián Tomašov (xtomas32)">

    <!-- CSRF Token -->
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <title>{{ config('app.name', 'Bug Tracker') }}</title>

    <!-- Styles -->
    <link href="{{ asset('css/app.css')     }}" rel="stylesheet">
    <link href="{{ asset('css/global.css') }}" rel="stylesheet">

    <!-- Tab Icon in browser -->
    <link href="{{ asset('fonts/favicon/favicon.ico') }}" rel="shortcut icon" type="image/x-icon">

    <!-- Custom Fonts -->
    <link href="{{ asset('font-awesome/css/font-awesome.min.css') }}" rel="stylesheet" type="text/css">

    <!-- Global tables -->
    <link href="{{ asset('css/datatables/dataTables.bootstrap4.css')}}" rel="stylesheet">

    <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
    <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>

</head>
<body>
<div id="app">
    <nav class="navbar navbar-default navbar-fixed-top">
        <div class="container">
            <div class="navbar-header">

                <!-- Branding Image -->
                <a class="navbar-brand" href="{{ url('/') }}">
                    <img src="{{asset('img/logo.png')}}" alt="Logo" height="50">
                </a>
            </div>

            <div class="collapse navbar-collapse" id="app-navbar-collapse">
                <!-- Left Side Of Navbar -->
                <ul class="nav navbar-nav">
                    &nbsp;

                </ul>

                <!-- Right Side Of Navbar -->
                <ul class="nav navbar-nav navbar-right">

                    <!-- Authentication Links -->
                    @guest
                        <li><a href="{{ route('login') }}">Login</a></li>
                        <li><a href="{{ route('register') }}">Register</a></li>
                    @endguest

                    @auth
                        <li class="dropdown">

                            <a href="#" id ="submit" class="btn btn-default dropdown-toggle" data-toggle="dropdown">Create &raquo;</a>
                            <ul class="dropdown-menu">
                                <li>
                                    <a href="{{url('/tickets/create')}}" id ="submit" class="grow btn btn-default" role="button">Create a Ticket</a>
                                </li>
                                <li>
                                    <a href="{{url('/patches/create')}}" id ="submit" class="grow btn btn-default" role="button">Create a Patch</a>
                                </li>
                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')
                                <li>
                                    <a href="{{url('/tests/create')}}" id ="submit" class="grow btn btn-default" role="button">Create a Test</a>
                                </li>
                                @endif
                                @if(Auth::user()->group == 'Admin')
                                <li>
                                    <a href="{{url('/modules/create')}}" id ="submit" class="grow btn btn-default" role="button">Create a Module</a>
                                </li>
                                @endif
                            </ul>
                        </li>
                            <li class="dropdown">
                                <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button"
                                   aria-expanded="false" aria-haspopup="true">
                                    {{ Auth::user()->name }} <span class="caret"></span>
                                </a>

                                <ul class="dropdown-menu">
                                    <li>
                                        <a href="{{ url('profile') }}">Edit profile</a>
                                    </li>
                                    <li>
                                        <a href="{{ route('logout') }}"
                                           onclick="event.preventDefault();
                                                     document.getElementById('logout-form').submit();">
                                            Logout
                                        </a>

                                        <form id="logout-form" action="{{ route('logout') }}" method="POST"
                                              style="display: none;">
                                            {{ csrf_field() }}
                                        </form>
                                    </li>
                                </ul>
                            </li>
                    @endauth

                </ul>
            </div>
            <div class="collapse navbar-collapse navbar-ex1-collapse">
                <ul class="nav navbar-nav side-nav">
                    <li>
                        <a href="{{ url('/home') }}"><i class="fa fa-fw fa-dashboard"></i> Dashboard</a>
                    </li>
                    <li>
                        <a href="{{ url('/bugs') }}"><i class="fa fa-fw fa-bug"></i> Bugs</a>
                    </li>
                    <li>
                        <a href="{{ url('/tickets') }}"><i class="fa fa-fw fa-edit"></i> Tickets</a>
                    </li>
                    <li>
                        <a href="{{ url('/patches') }}"><i class="fa fa-fw fa-cog"></i> Patches</a>
                    </li>
                    <li>
                        <a href="{{ url('/tests') }}"><i class="fa fa-fw fa-wrench"></i> Tests</a>
                    </li>
                    <li>
                        <a href="{{ url('/modules') }}"><i class="fa fa-fw fa-file-code-o"></i> Modules</a>
                    </li>
                    @auth
                    @if(Auth::user()->group == 'Admin')
                    <li>
                        <a href="{{ url('/admin') }}"><i class="fa fa-fw fa-user"></i> Admin page</a>
                    </li>
                    @endif
                    @endauth
                </ul>
            </div>
        </div>
    </nav>


    @if(request()->path() == 'login' or request()->path() == 'register')
        @yield('content')

    @else
        @auth
            @yield('content')
        @endauth

        @guest
                <div class="container">
                    <div class="row">
                        <div class="col-md-8 col-md-offset-2">
                            <div class="panel panel-default">
                                <div class="panel-heading text-center"><strong>Unauthorized access</strong></div>

                                <div class="panel-body text-center">
                                    <a href="{{url('/login')}}"><img src="{{asset('img/keep.png')}}" alt="Login" width="400"></a>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            @endguest
    @endif

</div>

<!-- Scripts -->
<script src="{{ asset('js/app.js') }}"></script>
</body>
</html>
