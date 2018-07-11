@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Test</div>

                    <div class="panel-body">
                        @if (!$test)
                            <h2>This test does not exist!</h2>
                        @else

                            <form class="form-horizontal" role="form" method="POST"
                                  action="{{ action('TestController@store') }}">
                                {{ csrf_field() }}


                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">ID of a test</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input type="text" class="form-control" name="id" value="{{$test->id}}"
                                                   readonly>

                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Result</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input type="number" step="0.1" class="form-control" name="result"
                                                   value="{{$test->hodnotenie}}"
                                                   @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') disabled @endif
                                            >
                                        </div>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Testing patch</label>
                                    <div class="col-md-6">
                                            <input id="fix_link" type="text" class="form-control" value="{{$test->id_patch}}" onclick="location.href=
                                            '/patches/{{$test->id_patch}}'" readonly>
                                    </div>


                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Test written by</label>
                                    <div class="col-md-6">
                                        <input id="fix_link" type="text" class="form-control" value="{{$test->napisal}}" onclick="location.href=
                                                '/profile/{{$test->napisal}}'" readonly>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Test approved by</label>
                                    <div class="col-md-6">
                                        <input id="fix_link" type="text" class="form-control" value="{{$test->name_schvaleny}}" onclick="location.href=
                                                '/profile/{{$test->name_schvaleny}}'" readonly>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Start date</label>
                                    <div class="col-md-6">
                                        <input type="date" class="form-control" name="start_date"
                                               value="{{$test->datum_zaciatku}}"
                                               @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') disabled @endif
                                        >
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">End date</label>
                                    <div class="col-md-6">
                                        <input type="date" class="form-control" name="end_date"
                                               value="{{$test->datum_konca}}"
                                               @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') disabled @endif
                                        >
                                    </div>
                                </div>

                                <div class="form-group">
                                <div class="col-md-6 col-md-offset-4">
                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')

                                    @if(!$test->name_schvaleny)
                                        <button type="submit" name="approve" value="{{ Auth::user()->name }}"
                                                class="btn btn-primary">Approve this test
                                        </button>
                                    @endif

                                    <button type="submit" name='user' value="{{ Auth::user()->name }}"
                                            class="btn btn-primary">Submit changes
                                    </button>
                                    <button type="reset" class="btn btn-default">Reset fields</button>
                                @endif
                                </div>
                                </div>

                                <hr>
                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Test code</label>
                                    <div class="col-md-6">
                                    <textarea class="form-control" rows="20" readonly>{{$test->code}}</textarea>
                                    </div>
                                </div>
                            </form>
                    </div>

                    @endif

                </div>
            </div>
        </div>
    </div>
    </div>
@endsection
