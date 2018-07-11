@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Edit profile</div>

                    <div class="panel-body">

                        <form class="form-horizontal" role="form" method="POST"
                              action="{{ action('ProfileController@store') }}">
                            {{ csrf_field() }}


                            <div class="form-group{{ $errors->has('name') ? ' has-error' : '' }}">
                                <label for="name" class="col-md-4 control-label">Username</label>

                                <div class="col-md-6">
                                    <input id="name" type="text" class="form-control" name="name"
                                           value="{{ Auth::user()->name }}" required autofocus>

                                    @if ($errors->has('name'))
                                        <span class="help-block">
                                        <strong>{{ $errors->first('name') }}</strong>
                                    </span>
                                    @endif
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="cele_meno" class="col-md-4 control-label">Full name</label>

                                <div class="col-md-6">
                                    <input id="cele_meno" type="text" class="form-control" name="cele_meno"
                                           required value=" {{ Auth::user()->cele_meno }}">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="vek" class="col-md-4 control-label">Age</label>

                                <div class="col-md-6">
                                    <input id="vek" type="text" class="form-control" name="vek"
                                           required value=" {{ Auth::user()->vek }}">
                                </div>
                            </div>

                            <div class="form-group">
                                <label for="languages" class="col-md-4 control-label">Programming Languages</label>

                                <div class="col-md-6">
                                    @foreach($inter as $item)
                                        <div class="checkbox">
                                            <label><input name="langs[]" type="checkbox" value="{{$item}}" checked>{{$item}}</label>
                                        </div>
                                    @endforeach

                                    @foreach($diff as $item)
                                        <div class="checkbox">
                                            <label><input name="langs[]" type="checkbox" value="{{$item}}">{{$item}}</label>
                                        </div>
                                    @endforeach
                                </div>
                            </div>


                            <div class="form-group{{ $errors->has('email') ? ' has-error' : '' }}">
                                <label for="email" class="col-md-4 control-label">E-Mail Address</label>

                                <div class="col-md-6">
                                    <input id="email" type="email" class="form-control" name="email"
                                           value={{ Auth::user()->email }} required>

                                    @if ($errors->has('email'))
                                        <span class="help-block">
                                        <strong>{{ $errors->first('email') }}</strong>
                                    </span>
                                    @endif
                                </div>
                            </div>


                            <div class="form-group">
                                <div class="col-md-6 col-md-offset-4">
                                    <button type="submit" class="btn btn-primary">
                                        Submit changes
                                    </button>
                                    <button type="submit" class="btn btn-default">
                                        Reset changes
                                    </button>
                                </div>
                            </div>


                        </form>

                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Initialize the plugin: -->
    <script type="text/javascript">
        $(document).ready(function () {
            $('#multi').multiselect();
        });
    </script>



@endsection
